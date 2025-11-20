#include "IndexBuffer.hpp"

#include <glad/glad.h>

MCEngine::IndexBuffer::IndexBuffer(size_t size) : BasicBuffer(static_cast<int>(size / sizeof(uint32_t)))
{
    glGenBuffers(1, &m_RendererID);
    Bind();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
    RendererCommand::GetError(std::string(FUNCTION_SIGNATURE));
    Unbind();
}

MCEngine::IndexBuffer::IndexBuffer(const void *data, size_t size)
    : BasicBuffer(static_cast<int>(size / sizeof(uint32_t)))
{
    CreateBuffer(data, size);
}

MCEngine::IndexBuffer::IndexBuffer(const std::vector<uint32_t> &indices) : BasicBuffer(static_cast<int>(indices.size()))
{
    CreateBuffer(indices.data(), indices.size() * sizeof(uint32_t));
}

MCEngine::IndexBuffer::~IndexBuffer()
{
    ENGINE_PROFILE_FUNCTION();

    glDeleteBuffers(1, &m_RendererID);
}

MCEngine::IndexBuffer::IndexBuffer(IndexBuffer &&other)
{
    ENGINE_PROFILE_FUNCTION();

    // Move data
    m_RendererID = other.m_RendererID;
    m_Count = other.m_Count;
    LOG_ENGINE_INFO("IndexBuffer move-assigned with ID: " + std::to_string(m_RendererID) +
                    " and count: " + std::to_string(m_Count));

    // Invalidate the moved-from object
    other.m_RendererID = 0;
}

MCEngine::IndexBuffer &MCEngine::IndexBuffer::operator=(IndexBuffer &&other)
{
    ENGINE_PROFILE_FUNCTION();

    if (this != &other)
    {
        if (m_RendererID != 0)
            glDeleteBuffers(1, &m_RendererID);

        // Move data
        m_RendererID = other.m_RendererID;
        m_Count = other.m_Count;
        LOG_ENGINE_INFO("IndexBuffer move-assigned with ID: " + std::to_string(m_RendererID) +
                        " and count: " + std::to_string(m_Count));

        // Invalidate the moved-from object
        other.m_RendererID = 0;
    }
    return *this;
}

void MCEngine::IndexBuffer::Bind() const
{
    ENGINE_PROFILE_FUNCTION();

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
}

void MCEngine::IndexBuffer::Unbind() const
{
    ENGINE_PROFILE_FUNCTION();

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void MCEngine::IndexBuffer::SetData(const void *data, size_t size, size_t offset)
{
    ENGINE_PROFILE_FUNCTION();

    Bind();
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, size, data);
    RendererCommand::GetError(std::string(FUNCTION_SIGNATURE));
    Unbind();
}

void MCEngine::IndexBuffer::CreateBuffer(const void *data, size_t size)
{
    ENGINE_PROFILE_FUNCTION();

    if (data == nullptr || size == 0)
    {
        LOG_ENGINE_WARN("Invalid IndexBuffer data or size");
        return;
    }

    glGenBuffers(1, &m_RendererID);
    Bind();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
    RendererCommand::GetError(std::string(FUNCTION_SIGNATURE));
    Unbind();

    LOG_ENGINE_INFO("IndexBuffer created with ID: " + std::to_string(m_RendererID) +
                    " and count: " + std::to_string(m_Count));
}