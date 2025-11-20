#include "RendererCommand.hpp"

#include <glad/glad.h>

void Engine::RendererCommand::Init()
{
    PROFILE_FUNCTION();

    EnableDepthTest();
    EnableBlend();
    EnableFaceCulling();
    EnableMultisampling();

    // Enable Gamma Correction
    glEnable(GL_FRAMEBUFFER_SRGB);

    // Wireframe mode
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    LOG_ENGINE_INFO("RendererCommand initialized");
}

void Engine::RendererCommand::GetError(const std::string &functionName)
{
    GLint error = glGetError();
    if (error != GL_NO_ERROR)
    {
        LOG_ENGINE_ASSERT("OpenGL Error: " + std::to_string(error) + " in " + functionName);
    }
}

void Engine::RendererCommand::SetClearColor(const glm::vec4 &color)
{
    glClearColor(color.r, color.g, color.b, color.a);
}

void Engine::RendererCommand::Clear() { glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); }

void Engine::RendererCommand::ClearColorBuffer() { glClear(GL_COLOR_BUFFER_BIT); }

void Engine::RendererCommand::ClearDepthBuffer() { glClear(GL_DEPTH_BUFFER_BIT); }

void Engine::RendererCommand::EnableDepthTest()
{
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
}

void Engine::RendererCommand::DisableDepthTest() { glDepthFunc(GL_LEQUAL); }

void Engine::RendererCommand::EnableBlend()
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Engine::RendererCommand::DisableBlend() { glDisable(GL_BLEND); }

void Engine::RendererCommand::EnableFaceCulling() { glEnable(GL_CULL_FACE); }

void Engine::RendererCommand::CullFrontFace() { glCullFace(GL_FRONT); }

void Engine::RendererCommand::CullBackFace() { glCullFace(GL_BACK); }

void Engine::RendererCommand::DisableFaceCulling() { glDisable(GL_CULL_FACE); }

void Engine::RendererCommand::EnableMultisampling() { glEnable(GL_MULTISAMPLE); }

void Engine::RendererCommand::DisableMultisampling() { glDisable(GL_MULTISAMPLE); }

void Engine::RendererCommand::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
{
    glViewport(x, y, width, height);
}