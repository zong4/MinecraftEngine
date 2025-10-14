#include "FrameBuffer.hpp"

#include <glad/glad.h>

MCEngine::FrameBuffer::FrameBuffer(FrameBufferType type, int width, int height, int samples)
    : m_Type(type), m_Width(width), m_Height(height)
{
    ENGINE_PROFILE_FUNCTION();

    glGenFramebuffers(1, &m_RendererID);
    glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);

    if (type != FrameBufferType::Multisample)
        BindBasicTexture(width, height);
    else
        BindMultiSampleTexture(width, height, samples);
    RendererCommand::GetError(std::string(FUNCTION_SIGNATURE));

    if (type != FrameBufferType::Depth)
        BindRenderBuffer(width, height, GL_DEPTH24_STENCIL8, samples);
    RendererCommand::GetError(std::string(FUNCTION_SIGNATURE));

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        LOG_ENGINE_ERROR("FrameBuffer is incomplete!");
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    LOG_ENGINE_INFO("FrameBuffer created with ID: " + std::to_string(m_RendererID) +
                    ", Type: " + std::to_string(static_cast<int>(m_Type)) + ", Width: " + std::to_string(width) +
                    ", Height: " + std::to_string(height) +
                    (type == FrameBufferType::Multisample ? ", Samples: " + std::to_string(samples) : ""));
}

MCEngine::FrameBuffer::~FrameBuffer()
{
    ENGINE_PROFILE_FUNCTION();

    glDeleteFramebuffers(1, &m_RendererID);
}

void MCEngine::FrameBuffer::Bind() const
{
    ENGINE_PROFILE_FUNCTION();

    glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
    glViewport(0, 0, m_Width, m_Height);
    RendererCommand::GetError(std::string(FUNCTION_SIGNATURE));
}

void MCEngine::FrameBuffer::Unbind() const
{
    ENGINE_PROFILE_FUNCTION();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    RendererCommand::GetError(std::string(FUNCTION_SIGNATURE));
}

void MCEngine::FrameBuffer::Blit(unsigned int resolveID) const
{
    ENGINE_PROFILE_FUNCTION();

    glBindFramebuffer(GL_READ_FRAMEBUFFER, m_RendererID);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, resolveID);
    glBlitFramebuffer(0, 0, m_Width, m_Height, 0, 0, m_Width, m_Height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
    RendererCommand::GetError(std::string(FUNCTION_SIGNATURE));
}

void MCEngine::FrameBuffer::Resize(int width, int height)
{
    ENGINE_PROFILE_FUNCTION();

    if (width == m_Width && height == m_Height)
        return;

    m_Width = width;
    m_Height = height;

    m_Texture->Resize(width, height);
    if (m_RenderBuffer)
        m_RenderBuffer->Resize(width, height);

    Bind();
    Unbind();
}

unsigned int MCEngine::FrameBuffer::PickPixel(int x, int y) const
{
    ENGINE_PROFILE_FUNCTION();

    glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
    glReadBuffer(GL_COLOR_ATTACHMENT0);

    unsigned int pixelData;
    glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_UNSIGNED_INT, &pixelData);
    RendererCommand::GetError(std::string(FUNCTION_SIGNATURE));

    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    return pixelData;
}

void MCEngine::FrameBuffer::BindBasicTexture(int width, int height)
{
    ENGINE_PROFILE_FUNCTION();

    if (m_Type == FrameBufferType::Color)
    {
        m_Texture = std::make_shared<Texture2D>(width, height, GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_Texture->GetRendererID(), 0);
    }
    else if (m_Type == FrameBufferType::Depth)
    {
        m_Texture = std::make_shared<Texture2D>(width, height, GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT, GL_FLOAT);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_Texture->GetRendererID(), 0);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
    }
    else if (m_Type == FrameBufferType::Integer)
    {
        m_Texture = std::make_shared<Texture2D>(width, height, GL_R32UI, GL_RED_INTEGER, GL_UNSIGNED_INT);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_Texture->GetRendererID(), 0);
    }
    else
    {
        LOG_ENGINE_ERROR("Invalid FrameBufferType for basic texture.");
    }
}

void MCEngine::FrameBuffer::BindMultiSampleTexture(int width, int height, int samples)
{
    ENGINE_PROFILE_FUNCTION();

    if (samples <= 0)
    {
        LOG_ENGINE_ERROR("Samples must be greater than 0 for multisample texture.");
        return;
    }

    m_Texture = std::make_shared<Texture2D>(width, height, samples);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, m_Texture->GetRendererID(),
                           0);
}

void MCEngine::FrameBuffer::BindRenderBuffer(int width, int height, unsigned int internalFormat, int samples)
{
    ENGINE_PROFILE_FUNCTION();

    m_RenderBuffer = std::make_shared<RenderBuffer>(width, height, internalFormat, samples);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER,
                              m_RenderBuffer->GetRendererID());
}