#include "TextureCube.hpp"

#include "../RendererProperty.hpp"
#include "OpenGLTextureCube.hpp"

std::shared_ptr<Engine::TextureCube> Engine::TextureCube::Create(const std::array<std::string, 6> &faces)
{
    switch (RendererProperty::GetInstance().GetAPI())
    {
    case RendererAPI::OpenGL:
        return std::make_shared<Engine::OpenGLTextureCube>(faces);
    case RendererAPI::Vulkan:
        LOG_ENGINE_ASSERT("Vulkan TextureCube is not implemented yet");
        return nullptr;
    default:
        LOG_ENGINE_ASSERT("Unknown RendererAPI");
        return nullptr;
    }
}

std::shared_ptr<Engine::TextureCube> Engine::TextureCube::WhiteTexture()
{
    switch (RendererProperty::GetInstance().GetAPI())
    {
    case RendererAPI::OpenGL:
        static std::shared_ptr<TextureCube> whiteTexture =
            std::make_shared<OpenGLTextureCube>(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
        return whiteTexture;
    case RendererAPI::Vulkan:
        LOG_ENGINE_ASSERT("Vulkan TextureCube is not implemented yet");
        return nullptr;
    default:
        LOG_ENGINE_ASSERT("Unknown RendererAPI");
        return nullptr;
    }
}