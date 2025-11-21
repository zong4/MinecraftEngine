#include "Texture2D.hpp"

#include "../RendererProperty.hpp"
#include "OpenGLTexture2D.hpp"
#include <glad/glad.h>

std::shared_ptr<Engine::Texture2D> Engine::Texture2D::Create(int width, int height, unsigned int internalFormat,
                                                             unsigned int format, unsigned int type, void *data)
{
    switch (RendererProperty::GetInstance().GetAPI())
    {
    case RendererAPI::OpenGL:
        return std::make_shared<Engine::OpenGLTexture2D>(width, height, internalFormat, format, type, data);
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
    switch (RendererProperty::GetInstance().GetAPI())
    {
    case RendererAPI::OpenGL:
        static std::shared_ptr<Texture2D> whiteTexture = std::make_shared<OpenGLTexture2D>(
            1, 1, GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE, new unsigned char[4]{255, 255, 255, 255});
        return whiteTexture;
    case RendererAPI::Vulkan:
        LOG_ENGINE_ASSERT("Vulkan Texture2D is not implemented yet");
        return nullptr;
    default:
        LOG_ENGINE_ASSERT("Unknown RendererAPI");
        return nullptr;
    }
}