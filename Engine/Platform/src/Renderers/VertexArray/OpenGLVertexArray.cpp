#include "OpenGLVertexArray.hpp"

#include <glad/glad.h>

GLenum ConvertRendererType(Engine::RendererType type)
{
    switch (type)
    {
    case Engine::RendererType::Points:
        return GL_POINTS;
    case Engine::RendererType::Lines:
        return GL_LINES;
    case Engine::RendererType::Triangles:
        return GL_TRIANGLES;
    default:
        LOG_ENGINE_ASSERT("Unknown RendererType");
        return GL_TRIANGLES;
    }
}

GLenum ConvertVertexAttributeType(Engine::VertexAttributeType type)
{
    switch (type)
    {
    case Engine::VertexAttributeType::Int:
        return GL_INT;
    case Engine::VertexAttributeType::UInt:
        return GL_UNSIGNED_INT;
    case Engine::VertexAttributeType::Float:
        return GL_FLOAT;
    default:
        LOG_ENGINE_ASSERT("Unknown VertexAttributeType");
        return GL_FLOAT;
    }
}

Engine::OpenGLVertexArray::OpenGLVertexArray(std::unique_ptr<VertexBuffer> vertexBuffer,
                                             const std::vector<VertexAttribute> &attributes,
                                             std::unique_ptr<IndexBuffer> indexBuffer, int instanceCount)
    : VertexArray(std::move(vertexBuffer), std::move(indexBuffer), instanceCount)
{
    glGenVertexArrays(1, &m_RendererID);
    SetVertexAttributes(attributes);
}

Engine::OpenGLVertexArray::~OpenGLVertexArray() { glDeleteVertexArrays(1, &m_RendererID); }

void Engine::OpenGLVertexArray::Render(RendererType renderType, int vertexCount) const
{
    PROFILE_FUNCTION();

    Bind();
    m_VertexBuffer->Bind();

    if (!m_IndexBuffer)
    {
        if (vertexCount == 0)
            vertexCount = m_VertexBuffer->GetVertexCount() / m_AttributeCount;
        m_InstanceCount == 1 ? glDrawArrays(ConvertRendererType(renderType), 0, vertexCount)
                             : glDrawArraysInstanced(ConvertRendererType(renderType), 0, vertexCount, m_InstanceCount);
        RendererCommand::GetError(std::string(FUNCTION_SIGNATURE));
    }
    else
    {
        m_IndexBuffer->Bind();
        if (vertexCount == 0)
            vertexCount = m_IndexBuffer->GetVertexCount();
        m_InstanceCount == 1 ? glDrawElements(ConvertRendererType(renderType), vertexCount, GL_UNSIGNED_INT, 0)
                             : glDrawElementsInstanced(ConvertRendererType(renderType), vertexCount, GL_UNSIGNED_INT, 0,
                                                       m_InstanceCount);
        RendererCommand::GetError(std::string(FUNCTION_SIGNATURE));
        m_IndexBuffer->Unbind();
    }

    m_VertexBuffer->Unbind();
    Unbind();
}

void Engine::OpenGLVertexArray::Bind() const { glBindVertexArray(m_RendererID); }

void Engine::OpenGLVertexArray::Unbind() const { glBindVertexArray(0); }

void Engine::OpenGLVertexArray::SetVertexAttributes(const std::vector<VertexAttribute> &attributes)
{
    PROFILE_FUNCTION();

    Bind();
    m_VertexBuffer->Bind();

    m_AttributeCount = static_cast<int>(attributes.size());
    for (const auto &attribute : attributes)
    {
        switch (attribute.type)
        {
        case Engine::VertexAttributeType::Int:
        case Engine::VertexAttributeType::UInt:
            glVertexAttribIPointer(attribute.location, attribute.count, ConvertVertexAttributeType(attribute.type),
                                   attribute.stride, attribute.offset);
            break;
        case Engine::VertexAttributeType::Float:
            glVertexAttribPointer(attribute.location, attribute.count, ConvertVertexAttributeType(attribute.type),
                                  attribute.normalized, static_cast<GLsizei>(attribute.stride), attribute.offset);
            glEnableVertexAttribArray(attribute.location);
            break;
        default:
            LOG_ENGINE_ASSERT("Unknown VertexAttributeType");
            break;
        }
        RendererCommand::GetError(std::string(FUNCTION_SIGNATURE));
    }

    m_VertexBuffer->Unbind();
    Unbind();

    LOG_ENGINE_INFO("VertexArray ID: " + std::to_string(m_RendererID) + " vertex attributes(" +
                    std::to_string(m_AttributeCount) + ") set/updated");
}