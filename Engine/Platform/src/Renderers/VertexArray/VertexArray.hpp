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
    static std::shared_ptr<VertexArray> Create(VertexBuffer &&vertexBuffer,
                                               const std::vector<VertexAttribute> &attributes,
                                               IndexBuffer &&indexBuffer = IndexBuffer(nullptr, 0),
                                               int instanceCount = 1);
    VertexArray(const VertexArray &) = delete;
    VertexArray &operator=(const VertexArray &) = delete;
    VertexArray(VertexArray &&);
    VertexArray &operator=(VertexArray &&);

    // Getters
    unsigned int GetRendererID() const { return m_RendererID; }
    VertexBuffer &GetVertexBuffer() { return m_VertexBuffer; }
    IndexBuffer &GetIndexBuffer() { return m_IndexBuffer; }

    // Setters
    void SetVertexBuffer(VertexBuffer &&vertexBuffer, const std::vector<VertexAttribute> &attributes);
    void SetIndexBuffer(IndexBuffer &&indexBuffer);
    void SetInstanceCount(int instanceCount) { m_InstanceCount = instanceCount; }

public:
    virtual void Render(RendererType renderType = RendererType::Triangles, size_t positionCount = 0) const = 0;

protected:
    unsigned int m_RendererID = 0;
    int m_AttributeCount = 0;
    VertexBuffer m_VertexBuffer;
    IndexBuffer m_IndexBuffer;

    // Instance rendering
    int m_InstanceCount = 1;

protected:
    VertexArray(VertexBuffer &&vertexBuffer, IndexBuffer &&indexBuffer = IndexBuffer(nullptr, 0), int instanceCount = 1)
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