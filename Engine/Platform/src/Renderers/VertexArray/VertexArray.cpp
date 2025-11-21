#include "VertexArray.hpp"

#include "../RendererProperty.hpp"
#include "OpenGLVertexArray.hpp"
#include <glad/glad.h>

std::shared_ptr<Engine::VertexArray> Engine::VertexArray::Create(VertexBuffer &&vertexBuffer,
                                                                 const std::vector<VertexAttribute> &attributes,
                                                                 IndexBuffer &&indexBuffer, int instanceCount)
{
    switch (Engine::RendererProperty::GetInstance().GetAPI())
    {
    case Engine::RendererAPI::OpenGL:
        return std::make_shared<Engine::OpenGLVertexArray>(std::move(vertexBuffer), attributes, std::move(indexBuffer),
                                                           instanceCount);
    case Engine::RendererAPI::Vulkan:
        LOG_ENGINE_ASSERT("Vulkan VertexArray is not implemented yet");
        return nullptr;
    default:
        LOG_ENGINE_ASSERT("Unknown RendererAPI");
        return nullptr;
    }
}

Engine::VertexArray::VertexArray(VertexArray &&other)
    : VertexArray(std::move(other.m_VertexBuffer), std::move(other.m_IndexBuffer), other.m_InstanceCount)
{
    m_RendererID = other.m_RendererID;
    m_AttributeCount = other.m_AttributeCount;
    LOG_ENGINE_TRACE("VertexArray move-constructed with ID: " + std::to_string(m_RendererID));

    // Invalidate the moved-from object
    other.m_RendererID = 0;
    other.m_AttributeCount = 0;
    other.m_InstanceCount = 1;
}

Engine::VertexArray &Engine::VertexArray::operator=(VertexArray &&other)
{
    if (this != &other)
    {
        if (m_RendererID != 0)
            glDeleteVertexArrays(1, &m_RendererID);

        // Move data
        m_RendererID = other.m_RendererID;
        m_AttributeCount = other.m_AttributeCount;
        m_IndexBuffer = std::move(other.m_IndexBuffer);
        m_VertexBuffer = std::move(other.m_VertexBuffer);
        m_InstanceCount = other.m_InstanceCount;
        LOG_ENGINE_TRACE("VertexArray move-assigned with ID: " + std::to_string(m_RendererID));

        // Invalidate the moved-from object
        other.m_RendererID = 0;
        other.m_AttributeCount = 0;
        other.m_InstanceCount = 1;
    }
    return *this;
}

void Engine::VertexArray::SetVertexBuffer(VertexBuffer &&vertexBuffer, const std::vector<VertexAttribute> &attributes)
{
    m_VertexBuffer = std::move(vertexBuffer);
    SetVertexAttributes(attributes);
    LOG_ENGINE_TRACE("VertexArray ID: " + std::to_string(m_RendererID) +
                     " set with new VertexBuffer ID: " + std::to_string(m_VertexBuffer.GetRendererID()));
}

void Engine::VertexArray::SetIndexBuffer(IndexBuffer &&indexBuffer)
{
    m_IndexBuffer = std::move(indexBuffer);
    LOG_ENGINE_TRACE("VertexArray ID: " + std::to_string(m_RendererID) +
                     " set with new IndexBuffer ID: " + std::to_string(m_IndexBuffer.GetRendererID()));
}
