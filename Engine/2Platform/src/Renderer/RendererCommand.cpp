#include "RendererCommand.hpp"

#include "RendererProperty.hpp"
#include <glad/glad.h>

void Engine::RendererCommand::Init()
{
    PROFILE_FUNCTION();

    // Preformance
    SetMultisampling(true);
    SetFaceCulling(CullingFace::Back);

    // Post-processing
    SetDepthTest(true);
    SetDepthTestFunction(DepthTestFunction::Less);
    SetDepthWrite(true);
    SetBlend(true);
    SetGammaCorrection(true);
    glEnable(GL_PROGRAM_POINT_SIZE);
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

void Engine::RendererCommand::SetFaceCulling(CullingFace face)
{
    switch (Engine::RendererProperty::GetInstance().GetAPI())
    {
    case Engine::RendererAPI::OpenGL:
        switch (face)
        {
        case CullingFace::None:
            glDisable(GL_CULL_FACE);
        case CullingFace::Front:
            glEnable(GL_CULL_FACE);
            glCullFace(GL_FRONT);
            break;
        case CullingFace::Back:
            glEnable(GL_CULL_FACE);
            glCullFace(GL_BACK);
            break;
        default:
            LOG_ENGINE_ASSERT("Unknown CullingFace");
            break;
        }
        break;
    case Engine::RendererAPI::Vulkan:
        LOG_ENGINE_ASSERT("Vulkan EnableFaceCulling is not implemented yet");
        break;
    default:
        LOG_ENGINE_ASSERT("Unknown RendererAPI");
        break;
    }
}

void Engine::RendererCommand::SetDepthTest(bool enabled)
{
    switch (Engine::RendererProperty::GetInstance().GetAPI())
    {
    case Engine::RendererAPI::OpenGL:
        enabled ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
        break;
    case Engine::RendererAPI::Vulkan:
        LOG_ENGINE_ASSERT("Vulkan DisableDepthTest is not implemented yet");
        break;
    default:
        LOG_ENGINE_ASSERT("Unknown RendererAPI");
        break;
    }
}

void Engine::RendererCommand::SetDepthWrite(bool enabled)
{
    switch (Engine::RendererProperty::GetInstance().GetAPI())
    {
    case Engine::RendererAPI::OpenGL:
        glDepthMask(enabled ? GL_TRUE : GL_FALSE);
        break;
    case Engine::RendererAPI::Vulkan:
        LOG_ENGINE_ASSERT("Vulkan SetDepthWrite is not implemented yet");
        break;
    default:
        LOG_ENGINE_ASSERT("Unknown RendererAPI");
        break;
    }
}

void Engine::RendererCommand::SetDepthTestFunction(DepthTestFunction function)
{
    switch (function)
    {
    case DepthTestFunction::Less:
        switch (Engine::RendererProperty::GetInstance().GetAPI())
        {
        case Engine::RendererAPI::OpenGL:
            glDepthFunc(GL_LESS);
            break;
        case Engine::RendererAPI::Vulkan:
            LOG_ENGINE_ASSERT("Vulkan SetDepthTestFunction is not implemented yet");
            break;
        default:
            LOG_ENGINE_ASSERT("Unknown RendererAPI");
            break;
        }
        break;
    case DepthTestFunction::LessEqual:
        switch (Engine::RendererProperty::GetInstance().GetAPI())
        {
        case Engine::RendererAPI::OpenGL:
            glDepthFunc(GL_LEQUAL);
            break;
        case Engine::RendererAPI::Vulkan:
            LOG_ENGINE_ASSERT("Vulkan SetDepthTestFunction is not implemented yet");
            break;
        default:
            LOG_ENGINE_ASSERT("Unknown RendererAPI");
            break;
        }
        break;
    case DepthTestFunction::Greater:
        switch (Engine::RendererProperty::GetInstance().GetAPI())
        {
        case Engine::RendererAPI::OpenGL:
            glDepthFunc(GL_GREATER);
            break;
        case Engine::RendererAPI::Vulkan:
            LOG_ENGINE_ASSERT("Vulkan SetDepthTestFunction is not implemented yet");
            break;
        default:
            LOG_ENGINE_ASSERT("Unknown RendererAPI");
            break;
        }
        break;
    case DepthTestFunction::GreaterEqual:
        switch (Engine::RendererProperty::GetInstance().GetAPI())
        {
        case Engine::RendererAPI::OpenGL:
            glDepthFunc(GL_GEQUAL);
            break;
        case Engine::RendererAPI::Vulkan:
            LOG_ENGINE_ASSERT("Vulkan SetDepthTestFunction is not implemented yet");
            break;
        default:
            LOG_ENGINE_ASSERT("Unknown RendererAPI");
            break;
        }
        break;
    case DepthTestFunction::Equal:
        switch (Engine::RendererProperty::GetInstance().GetAPI())
        {
        case Engine::RendererAPI::OpenGL:
            glDepthFunc(GL_EQUAL);
            break;
        case Engine::RendererAPI::Vulkan:
            LOG_ENGINE_ASSERT("Vulkan SetDepthTestFunction is not implemented yet");
            break;
        default:
            LOG_ENGINE_ASSERT("Unknown RendererAPI");
            break;
        }
        break;
    case DepthTestFunction::NotEqual:
        switch (Engine::RendererProperty::GetInstance().GetAPI())
        {
        case Engine::RendererAPI::OpenGL:
            glDepthFunc(GL_NOTEQUAL);
            break;
        case Engine::RendererAPI::Vulkan:
            LOG_ENGINE_ASSERT("Vulkan SetDepthTestFunction is not implemented yet");
            break;
        default:
            LOG_ENGINE_ASSERT("Unknown RendererAPI");
            break;
        }
        break;
    case DepthTestFunction::Always:
        switch (Engine::RendererProperty::GetInstance().GetAPI())
        {
        case Engine::RendererAPI::OpenGL:
            glDepthFunc(GL_ALWAYS);
            break;
        case Engine::RendererAPI::Vulkan:
            LOG_ENGINE_ASSERT("Vulkan SetDepthTestFunction is not implemented yet");
            break;
        default:
            LOG_ENGINE_ASSERT("Unknown RendererAPI");
            break;
        }
        break;
    case DepthTestFunction::Never:
        switch (Engine::RendererProperty::GetInstance().GetAPI())
        {
        case Engine::RendererAPI::OpenGL:
            glDepthFunc(GL_NEVER);
            break;
        case Engine::RendererAPI::Vulkan:
            LOG_ENGINE_ASSERT("Vulkan SetDepthTestFunction is not implemented yet");
            break;
        default:
            LOG_ENGINE_ASSERT("Unknown RendererAPI");
            break;
        }
        break;
    default:
        LOG_ENGINE_ASSERT("Unknown DepthTestFunction");
        break;
    }
}

void Engine::RendererCommand::SetBlend(bool enabled)
{
    switch (Engine::RendererProperty::GetInstance().GetAPI())
    {
    case Engine::RendererAPI::OpenGL:
        if (enabled)
        {
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        }
        else
        {
            glDisable(GL_BLEND);
        }
        break;
    case Engine::RendererAPI::Vulkan:
        LOG_ENGINE_ASSERT("Vulkan EnableBlend is not implemented yet");
        break;
    default:
        LOG_ENGINE_ASSERT("Unknown RendererAPI");
        break;
    }
}

void Engine::RendererCommand::SetGammaCorrection(bool enabled)
{
    switch (Engine::RendererProperty::GetInstance().GetAPI())
    {
    case Engine::RendererAPI::OpenGL:
        enabled ? glEnable(GL_FRAMEBUFFER_SRGB) : glDisable(GL_FRAMEBUFFER_SRGB);
        break;
    case Engine::RendererAPI::Vulkan:
        LOG_ENGINE_ASSERT("Vulkan EnableGammaCorrection is not implemented yet");
        break;
    default:
        LOG_ENGINE_ASSERT("Unknown RendererAPI");
        break;
    }
}

void Engine::RendererCommand::SetMultisampling(bool enabled)
{
    switch (Engine::RendererProperty::GetInstance().GetAPI())
    {
    case Engine::RendererAPI::OpenGL:
        enabled ? glEnable(GL_MULTISAMPLE) : glDisable(GL_MULTISAMPLE);
        break;
    case Engine::RendererAPI::Vulkan:
        LOG_ENGINE_ASSERT("Vulkan EnableMultisampling is not implemented yet");
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