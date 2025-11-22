#include "Texture2D.hpp"

#include "../RendererProperty.hpp"
#include "OpenGLTexture2D.hpp"

std::shared_ptr<Engine::Texture2D> Engine::Texture2D::Create(Texture2DType type, int width, int height, void *data)
{
    switch (RendererProperty::GetInstance().GetAPI())
    {
    case RendererAPI::OpenGL:
        return std::make_shared<Engine::OpenGLTexture2D>(type, width, height, data);
    case RendererAPI::Vulkan:
        LOG_ENGINE_ASSERT("Vulkan Texture2D is not implemented yet");
        return nullptr;
    default:
        LOG_ENGINE_ASSERT("Unknown RendererAPI");
        return nullptr;
    }
}

std::shared_ptr<Engine::Texture2D> Engine::Texture2D::Create(int width, int height, int samples)
{
    switch (RendererProperty::GetInstance().GetAPI())
    {
    case RendererAPI::OpenGL:
        return std::make_shared<Engine::OpenGLTexture2D>(width, height, samples);
    case RendererAPI::Vulkan:
        LOG_ENGINE_ASSERT("Vulkan Texture2D is not implemented yet");
        return nullptr;
    default:
        LOG_ENGINE_ASSERT("Unknown RendererAPI");
        return nullptr;
    }
}

std::shared_ptr<Engine::Texture2D> Engine::Texture2D::Create(const std::string &path)
{
    switch (RendererProperty::GetInstance().GetAPI())
    {
    case RendererAPI::OpenGL:
        return std::make_shared<Engine::OpenGLTexture2D>(path);
    case RendererAPI::Vulkan:
        LOG_ENGINE_ASSERT("Vulkan Texture2D is not implemented yet");
        return nullptr;
    default:
        LOG_ENGINE_ASSERT("Unknown RendererAPI");
        return nullptr;
    }
}

std::shared_ptr<Engine::Texture2D> Engine::Texture2D::WhiteTexture()
{
    static std::shared_ptr<Texture2D> whiteTexture = nullptr;
    if (whiteTexture)
        return whiteTexture;

    switch (RendererProperty::GetInstance().GetAPI())
    {
    case RendererAPI::OpenGL:
        whiteTexture = std::make_shared<OpenGLTexture2D>(Texture2DType::Color, 1, 1, new unsigned char[4]{255, 255, 255, 255});
        break;
    case RendererAPI::Vulkan:
        LOG_ENGINE_ASSERT("Vulkan Texture2D is not implemented yet");
        break;
    default:
        LOG_ENGINE_ASSERT("Unknown RendererAPI");
        break;
    }
    return whiteTexture;
}