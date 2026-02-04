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
    static std::shared_ptr<TextureCube> whiteTexture = nullptr;
    if (whiteTexture)
        return whiteTexture;

    unsigned char white[4] = {255, 255, 255, 255};
    switch (RendererProperty::GetInstance().GetAPI())
    {
    case RendererAPI::OpenGL:
        whiteTexture = std::make_shared<OpenGLTextureCube>(white);
        break;
    case RendererAPI::Vulkan:
        LOG_ENGINE_ASSERT("Vulkan TextureCube is not implemented yet");
        break;
    default:
        LOG_ENGINE_ASSERT("Unknown RendererAPI");
        break;
    }
    return whiteTexture;
}