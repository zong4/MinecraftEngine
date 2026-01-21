#include "RenderBuffer.hpp"

#include "../RendererProperty.hpp"
#include "OpenGLRenderBuffer.hpp"

std::unique_ptr<Engine::RenderBuffer> Engine::RenderBuffer::Create(int width, int height, unsigned int internalFormat,
                                                                   int samples)
{
    switch (Engine::RendererProperty::GetInstance().GetAPI())
    {
    case Engine::RendererAPI::OpenGL:
        return std::make_unique<Engine::OpenGLRenderBuffer>(width, height, internalFormat, samples);
    case Engine::RendererAPI::Vulkan:
        LOG_ENGINE_ASSERT("Vulkan RenderBuffer is not implemented yet");
        return nullptr;
    default:
        LOG_ENGINE_ASSERT("Unknown RendererAPI");
        return nullptr;
    }
}

void Engine::RenderBuffer::Resize(int width, int height)
{
    Bind(width, height);
    Unbind();
}