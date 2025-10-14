#pragma once

#include "Renderer/Buffer/BasicBuffer/IndexBuffer.hpp"
#include "Renderer/Buffer/BasicBuffer/VertexBuffer.hpp"
#include "Renderer/RendererCommand.hpp"

namespace MCEngine
{

enum class RendererType
{
    Points = 0,
    Lines = 1,
    Triangles = 4
};

struct VertexAttribute
{
    unsigned int location;
    unsigned int count;
    unsigned int type;
    bool normalized;
    size_t stride;
    const void *offset;
};

class VertexArray
{
public:
    VertexArray(VertexBuffer &&vertexBuffer, const std::vector<VertexAttribute> &attributes,
                IndexBuffer &&indexBuffer = IndexBuffer(nullptr, 0), int instanceCount = 1);
    ~VertexArray();

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
    void Render(RendererType renderType = RendererType::Triangles, size_t positionCount = 0) const;

private:
    unsigned int m_RendererID = 0;
    int m_AttributeCount = 0;
    VertexBuffer m_VertexBuffer;
    IndexBuffer m_IndexBuffer;

    // Instance rendering
    int m_InstanceCount;

protected:
    void Bind() const;
    void Unbind() const;

    void SetVertexAttributes(const std::vector<VertexAttribute> &attributes);
};

} // namespace MCEngine