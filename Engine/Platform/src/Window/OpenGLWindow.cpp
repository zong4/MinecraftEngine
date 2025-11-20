#include "OpenGLWindow.hpp"

#include "../Renderer/RendererCommand.hpp"
#include "../Renderer/RendererProperty.hpp"
#include <GLFW/glfw3.h>
#include <glad/glad.h>

void Engine::OpenGLWindow::Init()
{
    PROFILE_FUNCTION();

    // Initialize GLFW
    glfwInit();
    LOG_ENGINE_INFO("GLFW version: " + std::string(glfwGetVersionString()));

    // Set OpenGL version
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, Engine::RendererProperty::GetInstance().GetMajorVersion());
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, Engine::RendererProperty::GetInstance().GetMinorVersion());

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
    glfwGetFramebufferSize((GLFWwindow *)m_NativeWindow, &m_Property.FbWidth, &m_Property.FbHeight);
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
