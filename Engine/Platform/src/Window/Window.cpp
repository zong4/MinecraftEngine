#include "Window.hpp"

#include "../ImGui/ImGuiLayer.hpp"
#include "../Renderer/RendererCommand.hpp"
#include "../Renderer/RendererProperty.hpp"
#include "OpenGLWindow.hpp"
#include <GLFW/glfw3.h>

std::shared_ptr<Engine::Window> Engine::Window::Create(const WindowProperty &property)
{
    switch (Engine::RendererProperty::GetInstance().GetAPI())
    {
    case Engine::RendererAPI::OpenGL:
        return std::make_shared<OpenGLWindow>(property);
    case Engine::RendererAPI::Vulkan:
        LOG_ENGINE_ERROR("VulkanWindow is not implemented yet");
        return nullptr;
    default:
        LOG_ENGINE_ERROR("Unknown RendererAPI");
        return nullptr;
    }
}

bool Engine::Window::IsRunning() const
{
    return !glfwWindowShouldClose(static_cast<GLFWwindow *>(m_NativeWindow)) && m_Running;
}

void Engine::Window::SetVSync(bool enabled)
{
    PROFILE_FUNCTION();

    m_Property.VSync = enabled;
    enabled ? glfwSwapInterval(1) : glfwSwapInterval(0);
    LOG_ENGINE_INFO("VSync " + std::string(enabled ? "enabled" : "disabled"));
}

void Engine::Window::Update(float deltaTime)
{
    PROFILE_FUNCTION();

    glfwPollEvents();
    m_LayerStack.Update(deltaTime);
    glfwSwapBuffers(static_cast<GLFWwindow *>(m_NativeWindow));
}

void Engine::Window::Render()
{
    PROFILE_FUNCTION();

    std::dynamic_pointer_cast<ImGuiLayer>(m_LayerStack.GetImGuiLayer())->BeginRenderImGui();
    m_LayerStack.Render();
    std::dynamic_pointer_cast<ImGuiLayer>(m_LayerStack.GetImGuiLayer())->EndRenderImGui();
}

void Engine::Window::SetCallbacks()
{
    PROFILE_FUNCTION();

    glfwSetWindowUserPointer(static_cast<GLFWwindow *>(m_NativeWindow), this);

    glfwSetFramebufferSizeCallback(static_cast<GLFWwindow *>(m_NativeWindow),
                                   [](GLFWwindow *nativeWindow, int width, int height) {
                                       Window *window = static_cast<Window *>(glfwGetWindowUserPointer(nativeWindow));

                                       // Framebuffer size
                                       RendererCommand::SetViewport(0, 0, width, height);
                                       window->GetProperty().FbWidth = width;
                                       window->GetProperty().FbHeight = height;

                                       // Window size
                                       glfwGetWindowSize(nativeWindow, &width, &height);
                                       window->GetProperty().Width = width;
                                       window->GetProperty().Height = height;

                                       // Notify event
                                       WindowResizeEvent event(width, height);
                                       window->OnEvent(event);
                                   });

    glfwSetKeyCallback(static_cast<GLFWwindow *>(m_NativeWindow),
                       [](GLFWwindow *nativeWindow, int key, int scancode, int action, int mods) {
                           Window *window = static_cast<Window *>(glfwGetWindowUserPointer(nativeWindow));
                           KeyEvent event(key, action);
                           window->OnEvent(event);
                       });

    glfwSetMouseButtonCallback(static_cast<GLFWwindow *>(m_NativeWindow),
                               [](GLFWwindow *nativeWindow, int button, int action, int mods) {
                                   Window *window = static_cast<Window *>(glfwGetWindowUserPointer(nativeWindow));
                                   MouseButtonEvent event(button, action);
                                   window->OnEvent(event);
                               });

    glfwSetCursorPosCallback(static_cast<GLFWwindow *>(m_NativeWindow),
                             [](GLFWwindow *nativeWindow, double xPos, double yPos) {
                                 Window *window = static_cast<Window *>(glfwGetWindowUserPointer(nativeWindow));
                                 MouseMoveEvent event(xPos, yPos);
                                 window->OnEvent(event);
                             });

    glfwSetScrollCallback(static_cast<GLFWwindow *>(m_NativeWindow),
                          [](GLFWwindow *nativeWindow, double xOffset, double yOffset) {
                              Window *window = static_cast<Window *>(glfwGetWindowUserPointer(nativeWindow));
                              MouseScrollEvent event(xOffset, yOffset);
                              window->OnEvent(event);
                          });
}

void Engine::Window::Shutdown()
{
    PROFILE_FUNCTION();

    glfwDestroyWindow(static_cast<GLFWwindow *>(m_NativeWindow));
    glfwTerminate();
    LOG_ENGINE_INFO("Window destroyed");
}