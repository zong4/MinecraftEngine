#include "VertexArray.hpp"

#include <glad/glad.h>

MCEngine::VertexArray::VertexArray(VertexBuffer &&vertexBuffer, const std::vector<VertexAttribute> &attributes,
                                   IndexBuffer &&indexBuffer, int instanceCount)
    : m_VertexBuffer(std::move(vertexBuffer)), m_IndexBuffer(std::move(indexBuffer)), m_InstanceCount(instanceCount)
{
    ENGINE_PROFILE_FUNCTION();

    glGenVertexArrays(1, &m_RendererID);
    SetVertexAttributes(attributes);
    LOG_ENGINE_INFO("VertexArray created with ID: " + std::to_string(m_RendererID) +
                    ", VertexBuffer ID: " + std::to_string(m_VertexBuffer.GetRendererID()) +
                    ", IndexBuffer ID: " + std::to_string(m_IndexBuffer.GetRendererID()) +
                    ", Instance Count: " + std::to_string(m_InstanceCount));
}

MCEngine::VertexArray::~VertexArray() { glDeleteVertexArrays(1, &m_RendererID); }

MCEngine::VertexArray::VertexArray(VertexArray &&other)
    : m_RendererID(other.m_RendererID), m_AttributeCount(other.m_AttributeCount),
      m_IndexBuffer(std::move(other.m_IndexBuffer)), m_VertexBuffer(std::move(other.m_VertexBuffer)),
      m_InstanceCount(other.m_InstanceCount)
{
    ENGINE_PROFILE_FUNCTION();

    LOG_ENGINE_INFO("VertexArray moved with ID: " + std::to_string(m_RendererID));

    // Invalidate the moved-from object
    other.m_RendererID = 0;
    other.m_AttributeCount = 0;
    other.m_InstanceCount = 1;
}

MCEngine::VertexArray &MCEngine::VertexArray::operator=(VertexArray &&other)
{
    ENGINE_PROFILE_FUNCTION();

    if (this != &other)
    {
        glDeleteVertexArrays(1, &m_RendererID);

        // Move data
        m_RendererID = other.m_RendererID;
        m_AttributeCount = other.m_AttributeCount;
        m_IndexBuffer = std::move(other.m_IndexBuffer);
        m_VertexBuffer = std::move(other.m_VertexBuffer);
        m_InstanceCount = other.m_InstanceCount;
        LOG_ENGINE_INFO("VertexArray move-assigned with ID: " + std::to_string(m_RendererID));

        // Invalidate the moved-from object
        other.m_RendererID = 0;
        other.m_AttributeCount = 0;
        other.m_InstanceCount = 1;
    }
    return *this;
}

void MCEngine::VertexArray::SetVertexBuffer(VertexBuffer &&vertexBuffer, const std::vector<VertexAttribute> &attributes)
{
    ENGINE_PROFILE_FUNCTION();

    m_VertexBuffer = std::move(vertexBuffer);
    SetVertexAttributes(attributes);
    LOG_ENGINE_INFO("VertexArray ID: " + std::to_string(m_RendererID) +
                    " set with new VertexBuffer ID: " + std::to_string(m_VertexBuffer.GetRendererID()));
}

void MCEngine::VertexArray::SetIndexBuffer(IndexBuffer &&indexBuffer)
{
    ENGINE_PROFILE_FUNCTION();

    m_IndexBuffer = std::move(indexBuffer);
    LOG_ENGINE_INFO("VertexArray ID: " + std::to_string(m_RendererID) +
                    " set with new IndexBuffer ID: " + std::to_string(m_IndexBuffer.GetRendererID()));
}

void MCEngine::VertexArray::Render(RendererType renderType, size_t positionCount) const
{
    ENGINE_PROFILE_FUNCTION();

    Bind();
    m_VertexBuffer.Bind();

    if (m_IndexBuffer.GetRendererID() == 0)
    {
        size_t vertexCount = positionCount == 0 ? m_VertexBuffer.GetCount() / m_AttributeCount : positionCount;
        m_InstanceCount == 1 ? glDrawArrays(static_cast<GLenum>(renderType), 0, vertexCount)
                             : glDrawArraysInstanced(static_cast<GLenum>(renderType), 0, vertexCount, m_InstanceCount);
        RendererCommand::GetError(std::string(FUNCTION_SIGNATURE));
    }
    else
    {
        m_IndexBuffer.Bind();
        size_t vertexCount = positionCount == 0 ? m_IndexBuffer.GetCount() : positionCount;
        m_InstanceCount == 1 ? glDrawElements(static_cast<GLenum>(renderType), vertexCount, GL_UNSIGNED_INT, 0)
                             : glDrawElementsInstanced(static_cast<GLenum>(renderType), vertexCount, GL_UNSIGNED_INT, 0,
                                                       m_InstanceCount);
        RendererCommand::GetError(std::string(FUNCTION_SIGNATURE));
        m_IndexBuffer.Unbind();
    }
    m_VertexBuffer.Unbind();
    Unbind();
}

void MCEngine::VertexArray::Bind() const
{
    ENGINE_PROFILE_FUNCTION();

    glBindVertexArray(m_RendererID);
}

void MCEngine::VertexArray::Unbind() const
{
    ENGINE_PROFILE_FUNCTION();

    glBindVertexArray(0);
}

void MCEngine::VertexArray::SetVertexAttributes(const std::vector<VertexAttribute> &attributes)
{
    ENGINE_PROFILE_FUNCTION();

    Bind();
    m_VertexBuffer.Bind();
    m_AttributeCount = static_cast<int>(attributes.size());
    for (const auto &attribute : attributes)
    {
        if (attribute.type == GL_INT || attribute.type == GL_UNSIGNED_INT)
            glVertexAttribIPointer(attribute.location, attribute.count, attribute.type, attribute.stride,
                                   attribute.offset);
        else
            glVertexAttribPointer(attribute.location, attribute.count, attribute.type, attribute.normalized,
                                  static_cast<GLsizei>(attribute.stride), attribute.offset);
        glEnableVertexAttribArray(attribute.location);
        RendererCommand::GetError(std::string(FUNCTION_SIGNATURE));
    }
    m_VertexBuffer.Unbind();
    Unbind();

    LOG_ENGINE_INFO("VertexArray ID: " + std::to_string(m_RendererID) + " vertex attributes(" +
                    std::to_string(m_AttributeCount) + ") set/updated");
}