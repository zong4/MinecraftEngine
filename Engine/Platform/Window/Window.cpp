#include "Window.hpp"

#include "Event/EventDispatcher.hpp"
#include "Event/KeyEvent.hpp"
#include "Event/MouseEvent.hpp"
#include "Event/WindowEvent.hpp"
#include "Library/Event/MouseLibrary.hpp"
#include "Renderer/RendererCommand.hpp"
#include <GLFW/glfw3.h>
#include <glad/glad.h>

MCEngine::Window::Window(const WindowProperty &property) : m_Property(property) { Init(); }

MCEngine::Window::~Window() { Shutdown(); }

bool MCEngine::Window::IsRunning() const
{
    ENGINE_PROFILE_FUNCTION();

    return glfwWindowShouldClose(static_cast<GLFWwindow *>(m_NativeWindow)) || !m_Running;
}

void MCEngine::Window::SetVSync(bool enabled)
{
    ENGINE_PROFILE_FUNCTION();

    m_Property.VSync = enabled;
    enabled ? glfwSwapInterval(1) : glfwSwapInterval(0);
    LOG_ENGINE_INFO("VSync " + std::string(enabled ? "enabled" : "disabled"));
}

void MCEngine::Window::OnEvent(Event &event)
{
    ENGINE_PROFILE_FUNCTION();

    m_LayerStack.OnEvent(event);
}

void MCEngine::Window::Update(float deltaTime)
{
    ENGINE_PROFILE_FUNCTION();

    // Update
    m_LayerStack.Update(deltaTime);

    // Post-update
    glfwSwapBuffers(static_cast<GLFWwindow *>(m_NativeWindow));

    MCEngine::MouseLibrary::GetInstance().PostUpdate();
    glfwPollEvents();
}

void MCEngine::Window::Render()
{
    ENGINE_PROFILE_FUNCTION();

    // Pre-render
    MCEngine::RendererCommand::SetClearColor(m_Property.ClearColor);
    MCEngine::RendererCommand::Clear();

    // Render
    m_LayerStack.Render();
}

void MCEngine::Window::Init()
{
    ENGINE_PROFILE_FUNCTION();

    // Initialize GLFW
    glfwInit();
    LOG_ENGINE_INFO("GLFW version: " + std::string(glfwGetVersionString()));

    // Set OpenGL version
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, m_RendererAPIProperty.GetMajorVersion());
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, m_RendererAPIProperty.GetMinorVersion());

    // Other window hints
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 4);

    // Create window
    m_NativeWindow = glfwCreateWindow(m_Property.Width, m_Property.Height, m_Property.Title.c_str(), nullptr, nullptr);
    if (!m_NativeWindow)
    {
        LOG_ENGINE_ERROR("Failed to create GLFW window");
        glfwTerminate();
    }
    LOG_ENGINE_INFO("GLFW window created: " + m_Property.Title);

    // Make context current
    glfwMakeContextCurrent(static_cast<GLFWwindow *>(m_NativeWindow));
    SetCallbacks();
    SetVSync(m_Property.VSync);

    // Initialize GLAD
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    LOG_ENGINE_INFO("OpenGL version: " + std::string((const char *)glGetString(GL_VERSION)));

    // MSAA samples
    int samples = 0;
    glGetIntegerv(GL_SAMPLES, &samples);
    LOG_ENGINE_INFO("MSAA samples = " + std::to_string(samples));

    // Initialize Renderer
    RendererCommand::Init();
}

void MCEngine::Window::Shutdown()
{
    ENGINE_PROFILE_FUNCTION();

    glfwDestroyWindow(static_cast<GLFWwindow *>(m_NativeWindow));
    glfwTerminate();
    LOG_ENGINE_INFO("Window destroyed");
}

void MCEngine::Window::SetCallbacks()
{
    ENGINE_PROFILE_FUNCTION();

    glfwSetWindowUserPointer(static_cast<GLFWwindow *>(m_NativeWindow), this);

    glfwSetFramebufferSizeCallback(static_cast<GLFWwindow *>(m_NativeWindow),
                                   [](GLFWwindow *nativeWindow, int width, int height) {
                                       // Framebuffer size
                                       glViewport(0, 0, width, height);

                                       // Window size
                                       glfwGetWindowSize(nativeWindow, &width, &height);

                                       // OnEvent
                                       Window *window = static_cast<Window *>(glfwGetWindowUserPointer(nativeWindow));
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
