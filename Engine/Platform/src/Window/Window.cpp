#include "Window.hpp"

#include "Event/Input.hpp"
#include "Renderer/RendererCommand.hpp"
#include <GLFW/glfw3.h>
#include <glad/glad.h>

bool Engine::Window::IsRunning() const
{
    return !glfwWindowShouldClose(static_cast<GLFWwindow *>(m_NativeWindow)) && m_Running;
}

void Engine::Window::SetVSync(bool enabled)
{
    ENGINE_PROFILE_FUNCTION();

    m_Property.VSync = enabled;
    enabled ? glfwSwapInterval(1) : glfwSwapInterval(0);
    LOG_ENGINE_INFO("VSync " + std::string(enabled ? "enabled" : "disabled"));
}

void Engine::Window::OnEvent(Event &event)
{
    ENGINE_PROFILE_FUNCTION();

    // Store key states in KeyCodeLibrary
    if (!event.IsHandled())
    {
        Engine::EventDispatcher dispatcher(event);

        // KeyEvent
        dispatcher.Dispatch<Engine::KeyEvent>([this](Engine::KeyEvent &event) {
            Engine::Input::GetInstance().SetKeyAction(event.GetCode(), event.GetAction());
            return false;
        });

        // MouseEvent
        {
            dispatcher.Dispatch<Engine::MouseButtonEvent>([this](Engine::MouseButtonEvent &event) {
                Engine::Input::GetInstance().SetKeyAction(event.GetCode(), event.GetAction());
                return false;
            });
            dispatcher.Dispatch<Engine::MouseMoveEvent>([this](Engine::MouseMoveEvent &event) {
                Engine::Input::GetInstance().SetPosition(event.GetX(), event.GetY());
                return false;
            });
            dispatcher.Dispatch<Engine::MouseScrollEvent>([this](Engine::MouseScrollEvent &event) {
                Engine::Input::GetInstance().SetScrollOffset(event.GetXOffset(), event.GetYOffset());
                return false;
            });
        }
    }

    m_LayerStack.OnEvent(event);
}

void Engine::Window::Update(float deltaTime)
{
    ENGINE_PROFILE_FUNCTION();

    // Pre-update
    Input::GetInstance().Clear();
    glfwPollEvents();

    // Update
    m_LayerStack.Update(deltaTime);

    // Post-update
    glfwSwapBuffers(static_cast<GLFWwindow *>(m_NativeWindow));
}

void Engine::Window::Init()
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
        LOG_ENGINE_ASSERT("Failed to create GLFW window");
        glfwTerminate();
    }
    LOG_ENGINE_INFO("GLFW window created: " + m_Property.Title);

    glfwGetFramebufferSize((GLFWwindow *)m_NativeWindow, &m_Property.FbWidth, &m_Property.FbHeight);

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

void Engine::Window::Shutdown()
{
    ENGINE_PROFILE_FUNCTION();

    glfwDestroyWindow(static_cast<GLFWwindow *>(m_NativeWindow));
    glfwTerminate();
    LOG_ENGINE_INFO("Window destroyed");
}

void Engine::Window::SetCallbacks()
{
    ENGINE_PROFILE_FUNCTION();

    glfwSetWindowUserPointer(static_cast<GLFWwindow *>(m_NativeWindow), this);

    glfwSetFramebufferSizeCallback(static_cast<GLFWwindow *>(m_NativeWindow),
                                   [](GLFWwindow *nativeWindow, int width, int height) {
                                       Window *window = static_cast<Window *>(glfwGetWindowUserPointer(nativeWindow));

                                       // Framebuffer size
                                       glViewport(0, 0, width, height);
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
