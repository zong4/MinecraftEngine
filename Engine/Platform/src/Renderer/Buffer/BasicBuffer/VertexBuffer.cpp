#include "VertexBuffer.hpp"

#include <glad/glad.h>

MCEngine::VertexBuffer::VertexBuffer(size_t size) : BasicBuffer(static_cast<int>(size / sizeof(float)))
{
    glGenBuffers(1, &m_RendererID);
    Bind();
    glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
    RendererCommand::GetError(std::string(FUNCTION_SIGNATURE));
    Unbind();

    LOG_ENGINE_TRACE("VertexBuffer created with ID: " + std::to_string(m_RendererID) +
                     " and count: " + std::to_string(m_Count));
}

MCEngine::VertexBuffer::VertexBuffer(const void *data, size_t size)
    : BasicBuffer(static_cast<int>(size / sizeof(float)))
{
    CreateBuffer(data, size);
}

MCEngine::VertexBuffer::VertexBuffer(const std::vector<float> &vertices)
    : BasicBuffer(static_cast<int>(vertices.size()))
{
    CreateBuffer(vertices.data(), vertices.size() * sizeof(float));
}

MCEngine::VertexBuffer::~VertexBuffer()
{
    ENGINE_PROFILE_FUNCTION();

    glDeleteBuffers(1, &m_RendererID);
}

MCEngine::VertexBuffer::VertexBuffer(VertexBuffer &&other)
{
    ENGINE_PROFILE_FUNCTION();

    // Move data
    m_RendererID = other.m_RendererID;
    m_Count = other.m_Count;
    LOG_ENGINE_TRACE("VertexBuffer move-assigned with ID: " + std::to_string(m_RendererID) +
                     " and count: " + std::to_string(m_Count));

    // Invalidate the moved-from object
    other.m_RendererID = 0;
}

MCEngine::VertexBuffer &MCEngine::VertexBuffer::operator=(VertexBuffer &&other)
{
    ENGINE_PROFILE_FUNCTION();

    if (this != &other)
    {
        if (m_RendererID != 0)
            glDeleteBuffers(1, &m_RendererID);

        // Move data
        m_RendererID = other.m_RendererID;
        m_Count = other.m_Count;
        LOG_ENGINE_TRACE("VertexBuffer move-assigned with ID: " + std::to_string(m_RendererID) +
                         " and count: " + std::to_string(m_Count));

        // Invalidate the moved-from object
        other.m_RendererID = 0;
    }
    return *this;
}

void MCEngine::VertexBuffer::Bind() const
{
    ENGINE_PROFILE_FUNCTION();

    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
}

void MCEngine::VertexBuffer::Unbind() const
{
    ENGINE_PROFILE_FUNCTION();

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void MCEngine::VertexBuffer::SetData(const void *data, size_t size, size_t offset)
{
    ENGINE_PROFILE_FUNCTION();

    Bind();
    glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
    RendererCommand::GetError(std::string(FUNCTION_SIGNATURE));
    Unbind();
}

void MCEngine::VertexBuffer::CreateBuffer(const void *data, size_t size)
{
    ENGINE_PROFILE_FUNCTION();

    glGenBuffers(1, &m_RendererID);
    Bind();
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
    RendererCommand::GetError(std::string(FUNCTION_SIGNATURE));
    Unbind();

    LOG_ENGINE_TRACE("VertexBuffer created with ID: " + std::to_string(m_RendererID) +
                     " and count: " + std::to_string(m_Count));
}