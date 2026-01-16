#include "Window.hpp"

#include "../ImGuiLayer/ImGuiLayer.hpp"
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
        LOG_ENGINE_ASSERT("VulkanWindow is not implemented yet");
        return nullptr;
    default:
        LOG_ENGINE_ASSERT("Unknown RendererAPI");
        return nullptr;
    }
}

bool Engine::Window::IsRunning() const { return !glfwWindowShouldClose(static_cast<GLFWwindow *>(m_NativeWindow)); }

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
}

void Engine::Window::Render()
{
    PROFILE_FUNCTION();

    if (m_FirstFrame)
    {
        WindowResizeEvent event(m_Property.Width, m_Property.Height, m_Property.FbWidth, m_Property.FbHeight);
        OnEvent(event);
        m_FirstFrame = false;
    }

    std::dynamic_pointer_cast<ImGuiLayer>(m_LayerStack.GetTopLayer())
        ->BeginRenderImGui(); // ImGuiLayer is always the top layer
    m_LayerStack.Render();
    std::dynamic_pointer_cast<ImGuiLayer>(m_LayerStack.GetTopLayer())->EndRenderImGui();
    glfwSwapBuffers(static_cast<GLFWwindow *>(m_NativeWindow));
}

void Engine::Window::SetCallbacks()
{
    PROFILE_FUNCTION();

    glfwSetWindowUserPointer(static_cast<GLFWwindow *>(m_NativeWindow), this);

    glfwSetFramebufferSizeCallback(static_cast<GLFWwindow *>(m_NativeWindow),
                                   [](GLFWwindow *nativeWindow, int fbWidth, int fbHeight) {
                                       // Framebuffer size
                                       RendererCommand::SetViewport(0, 0, fbWidth, fbHeight);

                                       // Window size
                                       int width, height;
                                       glfwGetWindowSize(nativeWindow, &width, &height);
                                       Window *window = static_cast<Window *>(glfwGetWindowUserPointer(nativeWindow));
                                       window->GetProperty().Width = width;
                                       window->GetProperty().Height = height;
                                       window->GetProperty().FbWidth = fbWidth;
                                       window->GetProperty().FbHeight = fbHeight;

                                       // Notify event
                                       WindowResizeEvent event(width, height, fbWidth, fbHeight);
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