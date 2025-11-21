#include "OpenGLVertexBuffer.hpp"

#include "../../RendererCommand.hpp"
#include <glad/glad.h>

Engine::OpenGLVertexBuffer::OpenGLVertexBuffer(size_t size) : VertexBuffer(static_cast<int>(size / sizeof(float)))
{
    glGenBuffers(1, &m_RendererID);
    Bind();
    glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
    RendererCommand::GetError(std::string(FUNCTION_SIGNATURE));
    Unbind();
}

Engine::OpenGLVertexBuffer::OpenGLVertexBuffer(const void *data, size_t size)
    : VertexBuffer(static_cast<int>(size / sizeof(float)))
{
    CreateBuffer(data, size);
}

Engine::OpenGLVertexBuffer::OpenGLVertexBuffer(const std::initializer_list<float> &vertices)
    : VertexBuffer(static_cast<int>(vertices.size()))
{
    CreateBuffer(vertices.begin(), vertices.size() * sizeof(float));
}

Engine::OpenGLVertexBuffer::~OpenGLVertexBuffer() { glDeleteBuffers(1, &m_RendererID); }

void Engine::OpenGLVertexBuffer::Bind() const { glBindBuffer(GL_ARRAY_BUFFER, m_RendererID); }

void Engine::OpenGLVertexBuffer::Unbind() const { glBindBuffer(GL_ARRAY_BUFFER, 0); }

void Engine::OpenGLVertexBuffer::SetData(const void *data, size_t size, size_t offset)
{
    PROFILE_FUNCTION();

    Bind();
    glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
    RendererCommand::GetError(std::string(FUNCTION_SIGNATURE));
    Unbind();
}

void Engine::OpenGLVertexBuffer::CreateBuffer(const void *data, size_t size)
{
    PROFILE_FUNCTION();

    glGenBuffers(1, &m_RendererID);
    Bind();
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
    RendererCommand::GetError(std::string(FUNCTION_SIGNATURE));
    Unbind();

    LOG_ENGINE_TRACE("OpenGLVertexBuffer created with ID: " + std::to_string(m_RendererID) +
                     " and count: " + std::to_string(m_VertexCount));
}