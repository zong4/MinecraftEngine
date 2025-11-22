#include "FrameBuffer.hpp"

#include "../RendererProperty.hpp"
#include "OpenGLFrameBuffer.hpp"

std::shared_ptr<Engine::FrameBuffer> Engine::FrameBuffer::Create(Texture2DType type, int width, int height, int samples)
{
    switch (Engine::RendererProperty::GetInstance().GetAPI())
    {
    case Engine::RendererAPI::OpenGL:
        return std::make_shared<Engine::OpenGLFrameBuffer>(type, width, height, samples);
    case Engine::RendererAPI::Vulkan:
        LOG_ENGINE_ASSERT("Vulkan FrameBuffer is not implemented yet");
        return nullptr;
    default:
        LOG_ENGINE_ASSERT("Unknown RendererAPI");
        return nullptr;
    }
}

void Engine::FrameBuffer::Resize(int width, int height)
{
    PROFILE_FUNCTION();

    if (width == m_Width && height == m_Height)
        return;

    m_Texture->Resize(width, height);
    if (m_RenderBuffer)
        m_RenderBuffer->Resize(width, height);

    m_Width = width;
    m_Height = height;
    Bind();
    Unbind();
}
