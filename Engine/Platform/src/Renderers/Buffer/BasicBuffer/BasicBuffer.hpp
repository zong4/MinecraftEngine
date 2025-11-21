#pragma once

#include <Core.hpp>

namespace Engine
{

class BasicBuffer
{
public:
    BasicBuffer(int vertexCount = 0) : m_VertexCount(vertexCount) {}
    virtual ~BasicBuffer() = default;

    // Copy and move semantics
    BasicBuffer(const BasicBuffer &) = delete;
    BasicBuffer &operator=(const BasicBuffer &) = delete;

    // Getters
    unsigned int GetRendererID() const { return m_RendererID; }
    int GetVertexCount() const { return m_VertexCount; }

public:
    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;
    virtual void SetData(const void *data, size_t size, size_t offset) = 0;

protected:
    unsigned int m_RendererID = 0;
    int m_VertexCount;

protected:
    virtual void CreateBuffer(const void *data, size_t size) = 0;
};

} // namespace Engine