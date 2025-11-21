#include "IndexBuffer.hpp"

#include "../../RendererProperty.hpp"
#include "OpenGLIndexBuffer.hpp"

std::unique_ptr<Engine::IndexBuffer> Engine::IndexBuffer::Create(size_t size)
{
    switch (RendererProperty::GetInstance().GetAPI())
    {
    case RendererAPI::OpenGL:
        return std::make_unique<Engine::OpenGLIndexBuffer>(size);
    case RendererAPI::Vulkan:
        LOG_ENGINE_ASSERT("Vulkan IndexBuffer is not implemented yet");
        return nullptr;
    default:
        LOG_ENGINE_ASSERT("Unknown RendererAPI");
        return nullptr;
    }
}

std::unique_ptr<Engine::IndexBuffer> Engine::IndexBuffer::Create(const void *data)
{
    switch (RendererProperty::GetInstance().GetAPI())
    {
    case RendererAPI::OpenGL:
        return std::make_unique<Engine::OpenGLIndexBuffer>(data);
    case RendererAPI::Vulkan:
        LOG_ENGINE_ASSERT("Vulkan IndexBuffer is not implemented yet");
        return nullptr;
    default:
        LOG_ENGINE_ASSERT("Unknown RendererAPI");
        return nullptr;
    }
}

std::unique_ptr<Engine::IndexBuffer> Engine::IndexBuffer::Create(const std::initializer_list<uint32_t> &indices)
{
    switch (RendererProperty::GetInstance().GetAPI())
    {
    case RendererAPI::OpenGL:
        return std::make_unique<Engine::OpenGLIndexBuffer>(indices);
    case RendererAPI::Vulkan:
        LOG_ENGINE_ASSERT("Vulkan IndexBuffer is not implemented yet");
        return nullptr;
    default:
        LOG_ENGINE_ASSERT("Unknown RendererAPI");
        return nullptr;
    }
}