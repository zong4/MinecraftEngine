#include "RenderBuffer.hpp"

#include <glad/glad.h>

MCEngine::RenderBuffer::RenderBuffer(int width, int height, unsigned int internalFormat, int samples)
    : m_InternalFormat(internalFormat), m_Samples(samples)
{
    ENGINE_PROFILE_FUNCTION();

    glGenRenderbuffers(1, &m_RendererID);
    Bind(width, height);
    Unbind();
    LOG_ENGINE_INFO("RenderBuffer created with ID: " + std::to_string(m_RendererID) +
                    ", Width: " + std::to_string(width) + ", Height: " + std::to_string(height) +
                    ", InternalFormat: " + std::to_string(internalFormat) + ", Samples: " + std::to_string(samples));
}

MCEngine::RenderBuffer::~RenderBuffer()
{
    ENGINE_PROFILE_FUNCTION();

    glDeleteRenderbuffers(1, &m_RendererID);
}

void MCEngine::RenderBuffer::Resize(int width, int height)
{
    ENGINE_PROFILE_FUNCTION();

    Bind(width, height);
    Unbind();
}

void MCEngine::RenderBuffer::Bind(int width, int height) const
{
    ENGINE_PROFILE_FUNCTION();

    glBindRenderbuffer(GL_RENDERBUFFER, m_RendererID);
    m_Samples == 0 ? glRenderbufferStorage(GL_RENDERBUFFER, m_InternalFormat, width, height)
                   : glRenderbufferStorageMultisample(GL_RENDERBUFFER, m_Samples, m_InternalFormat, width, height);
    RendererCommand::GetError(std::string(FUNCTION_SIGNATURE));
}

void MCEngine::RenderBuffer::Unbind() const
{
    ENGINE_PROFILE_FUNCTION();

    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    RendererCommand::GetError(std::string(FUNCTION_SIGNATURE));
}