#include "UniformBuffer.hpp"

#include "../RendererProperty.hpp"
#include "OpenGLUniformBuffer.hpp"

std::shared_ptr<Engine::UniformBuffer> Engine::UniformBuffer::Create(size_t size, unsigned int binding)
{
    switch (Engine::RendererProperty::GetInstance().GetAPI())
    {
    case Engine::RendererAPI::OpenGL:
        return std::make_shared<Engine::OpenGLUniformBuffer>(size, binding);
    case Engine::RendererAPI::Vulkan:
        LOG_ENGINE_ASSERT("Vulkan UniformBuffer is not implemented yet");
        return nullptr;
    default:
        LOG_ENGINE_ASSERT("Unknown RendererAPI");
        return nullptr;
    }
}