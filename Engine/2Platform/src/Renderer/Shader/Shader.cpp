#include "Shader.hpp"

#include "../RendererProperty.hpp"
#include "OpenGLShader.hpp"

std::shared_ptr<Engine::Shader> Engine::Shader::Create(const std::string &vertexSource,
                                                       const std::string &fragmentSource,
                                                       const std::string &geometrySource)
{
    switch (RendererProperty::GetInstance().GetAPI())
    {
    case RendererAPI::OpenGL:
        return std::make_shared<Engine::OpenGLShader>(vertexSource, fragmentSource, geometrySource);
    case RendererAPI::Vulkan:
        LOG_ENGINE_ASSERT("Vulkan Shader is not implemented yet");
        return nullptr;
    default:
        LOG_ENGINE_ASSERT("Unknown RendererAPI");
        return nullptr;
    }
}