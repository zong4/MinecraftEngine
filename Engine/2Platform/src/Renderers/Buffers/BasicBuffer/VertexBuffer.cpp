#include "VertexBuffer.hpp"

#include "../../RendererProperty.hpp"
#include "OpenGLVertexBuffer.hpp"

std::unique_ptr<Engine::VertexBuffer> Engine::VertexBuffer::Create(size_t size)
{
    switch (RendererProperty::GetInstance().GetAPI())
    {
    case RendererAPI::OpenGL:
        return std::make_unique<Engine::OpenGLVertexBuffer>(size);
    case RendererAPI::Vulkan:
        LOG_ENGINE_ASSERT("Vulkan VertexBuffer is not implemented yet");
        return nullptr;
    default:
        LOG_ENGINE_ASSERT("Unknown RendererAPI");
        return nullptr;
    }
}

std::unique_ptr<Engine::VertexBuffer> Engine::VertexBuffer::Create(const void *data)
{
    switch (RendererProperty::GetInstance().GetAPI())
    {
    case RendererAPI::OpenGL:
        return std::make_unique<Engine::OpenGLVertexBuffer>(data);
    case RendererAPI::Vulkan:
        LOG_ENGINE_ASSERT("Vulkan VertexBuffer is not implemented yet");
        return nullptr;
    default:
        LOG_ENGINE_ASSERT("Unknown RendererAPI");
        return nullptr;
    }
}

std::unique_ptr<Engine::VertexBuffer> Engine::VertexBuffer::Create(const std::initializer_list<float> &vertices)
{
    switch (RendererProperty::GetInstance().GetAPI())
    {
    case RendererAPI::OpenGL:
        return std::make_unique<Engine::OpenGLVertexBuffer>(vertices);
    case RendererAPI::Vulkan:
        LOG_ENGINE_ASSERT("Vulkan VertexBuffer is not implemented yet");
        return nullptr;
    default:
        LOG_ENGINE_ASSERT("Unknown RendererAPI");
        return nullptr;
    }
}