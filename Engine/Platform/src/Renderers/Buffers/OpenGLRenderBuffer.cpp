#include "OpenGLRenderBuffer.hpp"

#include "../RendererCommand.hpp"
#include <glad/glad.h>

Engine::OpenGLRenderBuffer::OpenGLRenderBuffer(int width, int height, unsigned int internalFormat, int samples)
    : RenderBuffer(width, height, internalFormat, samples)
{
    glGenRenderbuffers(1, &m_RendererID);
    Resize(width, height);
}

Engine::OpenGLRenderBuffer::~OpenGLRenderBuffer() { glDeleteRenderbuffers(1, &m_RendererID); }

void Engine::OpenGLRenderBuffer::Bind(int width, int height) const
{
    glBindRenderbuffer(GL_RENDERBUFFER, m_RendererID);
    m_Samples == 0 ? glRenderbufferStorage(GL_RENDERBUFFER, m_InternalFormat, width, height)
                   : glRenderbufferStorageMultisample(GL_RENDERBUFFER, m_Samples, m_InternalFormat, width, height);
    RendererCommand::GetError(std::string(FUNCTION_SIGNATURE));
}

void Engine::OpenGLRenderBuffer::Unbind() const { glBindRenderbuffer(GL_RENDERBUFFER, 0); }