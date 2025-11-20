#include "RendererCommand.hpp"

#include "RendererProperty.hpp"
#include <glad/glad.h>

void Engine::RendererCommand::Init()
{
    PROFILE_FUNCTION();

    EnableDepthTest();
    EnableBlend();
    EnableFaceCulling();
    EnableMultisampling();
    EnableGammaCorrection();
}

void Engine::RendererCommand::GetError(const std::string &functionName)
{
    switch (Engine::RendererProperty::GetInstance().GetAPI())
    {
    case Engine::RendererAPI::OpenGL: {
        GLint error = glGetError();
        if (error != GL_NO_ERROR)
            LOG_ENGINE_ASSERT("OpenGL Error: " + std::to_string(error) + " in " + functionName);
        break;
    }
    case Engine::RendererAPI::Vulkan:
        LOG_ENGINE_ASSERT("Vulkan Error checking is not implemented yet");
        break;
    default:
        LOG_ENGINE_ASSERT("Unknown RendererAPI");
        break;
    }
}

void Engine::RendererCommand::SetClearColor(const glm::vec4 &color)
{
    switch (Engine::RendererProperty::GetInstance().GetAPI())
    {
    case Engine::RendererAPI::OpenGL:
        glClearColor(color.r, color.g, color.b, color.a);
        break;
    case Engine::RendererAPI::Vulkan:
        LOG_ENGINE_ASSERT("Vulkan SetClearColor is not implemented yet");
        break;
    default:
        LOG_ENGINE_ASSERT("Unknown RendererAPI");
        break;
    }
}

void Engine::RendererCommand::Clear()
{
    switch (Engine::RendererProperty::GetInstance().GetAPI())
    {
    case Engine::RendererAPI::OpenGL:
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        break;
    case Engine::RendererAPI::Vulkan:
        LOG_ENGINE_ASSERT("Vulkan Clear is not implemented yet");
        break;
    default:
        LOG_ENGINE_ASSERT("Unknown RendererAPI");
        break;
    }
}

void Engine::RendererCommand::ClearColorBuffer()
{
    switch (Engine::RendererProperty::GetInstance().GetAPI())
    {
    case Engine::RendererAPI::OpenGL:
        glClear(GL_COLOR_BUFFER_BIT);
        break;
    case Engine::RendererAPI::Vulkan:
        LOG_ENGINE_ASSERT("Vulkan ClearColorBuffer is not implemented yet");
        break;
    default:
        LOG_ENGINE_ASSERT("Unknown RendererAPI");
        break;
    }
}

void Engine::RendererCommand::ClearDepthBuffer()
{
    switch (Engine::RendererProperty::GetInstance().GetAPI())
    {
    case Engine::RendererAPI::OpenGL:
        glClear(GL_DEPTH_BUFFER_BIT);
        break;
    case Engine::RendererAPI::Vulkan:
        LOG_ENGINE_ASSERT("Vulkan ClearDepthBuffer is not implemented yet");
        break;
    default:
        LOG_ENGINE_ASSERT("Unknown RendererAPI");
        break;
    }
}

void Engine::RendererCommand::EnableDepthTest()
{
    switch (Engine::RendererProperty::GetInstance().GetAPI())
    {
    case Engine::RendererAPI::OpenGL:
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
        break;
    case Engine::RendererAPI::Vulkan:
        LOG_ENGINE_ASSERT("Vulkan EnableDepthTest is not implemented yet");
        break;
    default:
        LOG_ENGINE_ASSERT("Unknown RendererAPI");
        break;
    }
}

void Engine::RendererCommand::DisableDepthTest()
{
    switch (Engine::RendererProperty::GetInstance().GetAPI())
    {
    case Engine::RendererAPI::OpenGL:
        glDepthFunc(GL_LEQUAL);
        break;
    case Engine::RendererAPI::Vulkan:
        LOG_ENGINE_ASSERT("Vulkan DisableDepthTest is not implemented yet");
        break;
    default:
        LOG_ENGINE_ASSERT("Unknown RendererAPI");
        break;
    }
}

void Engine::RendererCommand::EnableBlend()
{
    switch (Engine::RendererProperty::GetInstance().GetAPI())
    {
    case Engine::RendererAPI::OpenGL:
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        break;
    case Engine::RendererAPI::Vulkan:
        LOG_ENGINE_ASSERT("Vulkan EnableBlend is not implemented yet");
        break;
    default:
        LOG_ENGINE_ASSERT("Unknown RendererAPI");
        break;
    }
}

void Engine::RendererCommand::DisableBlend()
{
    switch (Engine::RendererProperty::GetInstance().GetAPI())
    {
    case Engine::RendererAPI::OpenGL:
        glDisable(GL_BLEND);
        break;
    case Engine::RendererAPI::Vulkan:
        LOG_ENGINE_ASSERT("Vulkan DisableBlend is not implemented yet");
        break;
    default:
        LOG_ENGINE_ASSERT("Unknown RendererAPI");
        break;
    }
}

void Engine::RendererCommand::EnableFaceCulling()
{
    switch (Engine::RendererProperty::GetInstance().GetAPI())
    {
    case Engine::RendererAPI::OpenGL:
        glEnable(GL_CULL_FACE);
        break;
    case Engine::RendererAPI::Vulkan:
        LOG_ENGINE_ASSERT("Vulkan EnableFaceCulling is not implemented yet");
        break;
    default:
        LOG_ENGINE_ASSERT("Unknown RendererAPI");
        break;
    }
}

void Engine::RendererCommand::DisableFaceCulling()
{
    switch (Engine::RendererProperty::GetInstance().GetAPI())
    {
    case Engine::RendererAPI::OpenGL:
        glDisable(GL_CULL_FACE);
        break;
    case Engine::RendererAPI::Vulkan:
        LOG_ENGINE_ASSERT("Vulkan DisableFaceCulling is not implemented yet");
        break;
    default:
        LOG_ENGINE_ASSERT("Unknown RendererAPI");
        break;
    }
}

void Engine::RendererCommand::CullFrontFace()
{
    switch (Engine::RendererProperty::GetInstance().GetAPI())
    {
    case Engine::RendererAPI::OpenGL:
        glCullFace(GL_FRONT);
        break;
    case Engine::RendererAPI::Vulkan:
        LOG_ENGINE_ASSERT("Vulkan CullFrontFace is not implemented yet");
        break;
    default:
        LOG_ENGINE_ASSERT("Unknown RendererAPI");
        break;
    }
}

void Engine::RendererCommand::CullBackFace()
{
    switch (Engine::RendererProperty::GetInstance().GetAPI())
    {
    case Engine::RendererAPI::OpenGL:
        glCullFace(GL_BACK);
        break;
    case Engine::RendererAPI::Vulkan:
        LOG_ENGINE_ASSERT("Vulkan CullBackFace is not implemented yet");
        break;
    default:
        LOG_ENGINE_ASSERT("Unknown RendererAPI");
        break;
    }
}

void Engine::RendererCommand::EnableMultisampling()
{
    switch (Engine::RendererProperty::GetInstance().GetAPI())
    {
    case Engine::RendererAPI::OpenGL:
        glEnable(GL_MULTISAMPLE);
        break;
    case Engine::RendererAPI::Vulkan:
        LOG_ENGINE_ASSERT("Vulkan EnableMultisampling is not implemented yet");
        break;
    default:
        LOG_ENGINE_ASSERT("Unknown RendererAPI");
        break;
    }
}

void Engine::RendererCommand::DisableMultisampling()
{
    switch (Engine::RendererProperty::GetInstance().GetAPI())
    {
    case Engine::RendererAPI::OpenGL:
        glDisable(GL_MULTISAMPLE);
        break;
    case Engine::RendererAPI::Vulkan:
        LOG_ENGINE_ASSERT("Vulkan DisableMultisampling is not implemented yet");
        break;
    default:
        LOG_ENGINE_ASSERT("Unknown RendererAPI");
        break;
    }
}

void Engine::RendererCommand::EnableGammaCorrection()
{
    switch (Engine::RendererProperty::GetInstance().GetAPI())
    {
    case Engine::RendererAPI::OpenGL:
        glEnable(GL_FRAMEBUFFER_SRGB);
        break;
    case Engine::RendererAPI::Vulkan:
        LOG_ENGINE_ASSERT("Vulkan EnableGammaCorrection is not implemented yet");
        break;
    default:
        LOG_ENGINE_ASSERT("Unknown RendererAPI");
        break;
    }
}

void Engine::RendererCommand::DisableGammaCorrection()
{
    switch (Engine::RendererProperty::GetInstance().GetAPI())
    {
    case Engine::RendererAPI::OpenGL:
        glDisable(GL_FRAMEBUFFER_SRGB);
        break;
    case Engine::RendererAPI::Vulkan:
        LOG_ENGINE_ASSERT("Vulkan DisableGammaCorrection is not implemented yet");
        break;
    default:
        LOG_ENGINE_ASSERT("Unknown RendererAPI");
        break;
    }
}

void Engine::RendererCommand::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
{
    switch (Engine::RendererProperty::GetInstance().GetAPI())
    {
    case Engine::RendererAPI::OpenGL:
        glViewport(x, y, width, height);
        break;
    case Engine::RendererAPI::Vulkan:
        LOG_ENGINE_ASSERT("Vulkan SetViewport is not implemented yet");
        break;
    default:
        LOG_ENGINE_ASSERT("Unknown RendererAPI");
        break;
    }
}