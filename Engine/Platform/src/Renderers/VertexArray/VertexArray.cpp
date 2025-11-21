#include "VertexArray.hpp"

#include "../RendererProperty.hpp"
#include "OpenGLVertexArray.hpp"

std::shared_ptr<Engine::VertexArray> Engine::VertexArray::Create(std::unique_ptr<VertexBuffer> vertexBuffer,
                                                                 const std::vector<VertexAttribute> &attributes,
                                                                 std::unique_ptr<IndexBuffer> indexBuffer,
                                                                 int instanceCount)
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

void Engine::VertexArray::SetVertexBuffer(std::unique_ptr<VertexBuffer> vertexBuffer,
                                          const std::vector<VertexAttribute> &attributes)
{
    m_VertexBuffer = std::move(vertexBuffer);
    SetVertexAttributes(attributes);
    LOG_ENGINE_TRACE("VertexArray ID: " + std::to_string(m_RendererID) +
                     " set with new VertexBuffer ID: " + std::to_string(m_VertexBuffer->GetRendererID()));
}

void Engine::VertexArray::SetIndexBuffer(std::unique_ptr<IndexBuffer> indexBuffer)
{
    m_IndexBuffer = std::move(indexBuffer);
    LOG_ENGINE_TRACE("VertexArray ID: " + std::to_string(m_RendererID) +
                     " set with new IndexBuffer ID: " + std::to_string(m_IndexBuffer->GetRendererID()));
}
