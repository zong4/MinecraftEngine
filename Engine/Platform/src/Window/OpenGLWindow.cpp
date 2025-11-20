#include "OpenGLWindow.hpp"

#include "../Renderers/RendererCommand.hpp"
#include "../Renderers/RendererProperty.hpp"
#include <GLFW/glfw3.h>
#include <glad/glad.h>

void Engine::OpenGLWindow::Init()
{
    PROFILE_FUNCTION();

    // Initialize GLFW
    glfwInit();
    LOG_ENGINE_INFO("GLFW version: " + std::string(glfwGetVersionString()));

    // Set OpenGL context hints
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, Engine::RendererProperty::GetInstance().GetMajorVersion());
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, Engine::RendererProperty::GetInstance().GetMinorVersion());
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 4);

    // Create GLFW window
    m_NativeWindow = glfwCreateWindow(m_Property.Width, m_Property.Height, m_Property.Title.c_str(), nullptr, nullptr);
    if (!m_NativeWindow)
    {
        LOG_ENGINE_ASSERT("Failed to create GLFW window");
        glfwTerminate();
    }
    LOG_ENGINE_INFO("GLFW window created: " + m_Property.Title);

    // Make context current
    glfwMakeContextCurrent(static_cast<GLFWwindow *>(m_NativeWindow));

    // Initialize GLAD
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    LOG_ENGINE_INFO("OpenGL version: " + std::string((const char *)glGetString(GL_VERSION)));

    // Use property to set
    glfwGetFramebufferSize((GLFWwindow *)m_NativeWindow, &m_Property.FbWidth, &m_Property.FbHeight);
    RendererCommand::SetViewport(0, 0, m_Property.FbWidth, m_Property.FbHeight);
    SetVSync(m_Property.VSync);
    SetCallbacks();

    // MSAA samples
    int samples = 0;
    glGetIntegerv(GL_SAMPLES, &samples);
    LOG_ENGINE_INFO("MSAA samples = " + std::to_string(samples));

    // Initialize Renderer
    RendererCommand::Init();
}
