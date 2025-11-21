#pragma once

#include "../Buffer/IndexBuffer.hpp"
#include "../Buffer/VertexBuffer.hpp"
#include "../RendererCommand.hpp"

namespace Engine
{

enum class RendererType
{
    Points,
    Lines,
    Triangles
};

enum class VertexAttributeType
{
    Int,
    UInt,
    Float,
};

struct VertexAttribute
{
    unsigned int location;
    unsigned int count;
    VertexAttributeType type;
    bool normalized;
    size_t stride;
    const void *offset;
};

class VertexArray
{
public:
    static std::shared_ptr<VertexArray> Create(
        std::unique_ptr<VertexBuffer> vertexBuffer, const std::vector<VertexAttribute> &attributes,
        std::unique_ptr<IndexBuffer> indexBuffer = std::make_unique<IndexBuffer>(nullptr, 0), int instanceCount = 1);

    // Copy
    VertexArray(const VertexArray &) = delete;
    VertexArray &operator=(const VertexArray &) = delete;

    // Getters
    unsigned int GetRendererID() const { return m_RendererID; }
    std::unique_ptr<VertexBuffer> &GetVertexBuffer() { return m_VertexBuffer; }
    std::unique_ptr<IndexBuffer> &GetIndexBuffer() { return m_IndexBuffer; }

    // Setters
    void SetVertexBuffer(std::unique_ptr<VertexBuffer> vertexBuffer, const std::vector<VertexAttribute> &attributes);
    void SetIndexBuffer(std::unique_ptr<IndexBuffer> indexBuffer);
    void SetInstanceCount(int instanceCount) { m_InstanceCount = instanceCount; }

public:
    virtual void Render(RendererType renderType = RendererType::Triangles, size_t positionCount = 0) const = 0;

protected:
    unsigned int m_RendererID = 0;
    int m_AttributeCount = 0;
    std::unique_ptr<VertexBuffer> m_VertexBuffer;
    std::unique_ptr<IndexBuffer> m_IndexBuffer;

    // Instance rendering
    int m_InstanceCount = 1;

protected:
    VertexArray(std::unique_ptr<VertexBuffer> vertexBuffer,
                std::unique_ptr<IndexBuffer> indexBuffer = std::make_unique<IndexBuffer>(nullptr, 0),
                int instanceCount = 1)
        : m_VertexBuffer(std::move(vertexBuffer)), m_IndexBuffer(std::move(indexBuffer)), m_InstanceCount(instanceCount)
    {
    }
    virtual ~VertexArray() = default;

protected:
    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;
    virtual void SetVertexAttributes(const std::vector<VertexAttribute> &attributes) = 0;
};

} // namespace Engine