#include "RendererCommand.hpp"

#include <glad/glad.h>

void MCEngine::RendererCommand::Init()
{
    ENGINE_PROFILE_FUNCTION();

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

void MCEngine::RendererCommand::GetError(const std::string &functionName)
{
    GLint error = glGetError();
    if (error != GL_NO_ERROR)
    {
        LOG_ENGINE_ERROR("OpenGL Error: " + std::to_string(error) + " in " + functionName);
    }
}

void MCEngine::RendererCommand::SetClearColor(const glm::vec4 &color)
{
    glClearColor(color.r, color.g, color.b, color.a);
}

void MCEngine::RendererCommand::Clear() { glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); }

void MCEngine::RendererCommand::ClearColorBuffer() { glClear(GL_COLOR_BUFFER_BIT); }

void MCEngine::RendererCommand::ClearDepthBuffer() { glClear(GL_DEPTH_BUFFER_BIT); }

void MCEngine::RendererCommand::EnableDepthTest()
{
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
}

void MCEngine::RendererCommand::DisableDepthTest() { glDepthFunc(GL_LEQUAL); }

void MCEngine::RendererCommand::EnableBlend()
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void MCEngine::RendererCommand::DisableBlend() { glDisable(GL_BLEND); }

void MCEngine::RendererCommand::EnableFaceCulling() { glEnable(GL_CULL_FACE); }

void MCEngine::RendererCommand::CullFrontFace() { glCullFace(GL_FRONT); }

void MCEngine::RendererCommand::CullBackFace() { glCullFace(GL_BACK); }

void MCEngine::RendererCommand::DisableFaceCulling() { glDisable(GL_CULL_FACE); }

void MCEngine::RendererCommand::EnableMultisampling() { glEnable(GL_MULTISAMPLE); }

void MCEngine::RendererCommand::DisableMultisampling() { glDisable(GL_MULTISAMPLE); }