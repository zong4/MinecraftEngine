#include "OpenGLIndexBuffer.hpp"

#include "../../RendererCommand.hpp"
#include <glad/glad.h>

Engine::OpenGLIndexBuffer::OpenGLIndexBuffer(size_t size) : IndexBuffer(static_cast<int>(size / sizeof(uint32_t)))
{
    glGenBuffers(1, &m_RendererID);
    Bind();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
    RendererCommand::GetError(std::string(FUNCTION_SIGNATURE));
    Unbind();
}

Engine::OpenGLIndexBuffer::OpenGLIndexBuffer(const std::initializer_list<uint32_t> &indices)
    : IndexBuffer(static_cast<int>(indices.size()))
{
    CreateBuffer(indices.begin(), indices.size() * sizeof(uint32_t));
}

Engine::OpenGLIndexBuffer::~OpenGLIndexBuffer() { glDeleteBuffers(1, &m_RendererID); }

void Engine::OpenGLIndexBuffer::Bind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID); }

void Engine::OpenGLIndexBuffer::Unbind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); }

void Engine::OpenGLIndexBuffer::SetData(const void *data, size_t size, size_t offset)
{
    PROFILE_FUNCTION();

    Bind();
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, size, data);
    RendererCommand::GetError(std::string(FUNCTION_SIGNATURE));
    Unbind();
}

void Engine::OpenGLIndexBuffer::CreateBuffer(const void *data, size_t size)
{
    PROFILE_FUNCTION();

    if (data == nullptr || size == 0)
    {
        LOG_ENGINE_ERROR("Invalid OpenGLIndexBuffer data or size");
        return;
    }

    glGenBuffers(1, &m_RendererID);
    Bind();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
    RendererCommand::GetError(std::string(FUNCTION_SIGNATURE));
    Unbind();

    LOG_ENGINE_TRACE("OpenGLIndexBuffer created with ID: " + std::to_string(m_RendererID) +
                     " and count: " + std::to_string(m_VertexCount));
}