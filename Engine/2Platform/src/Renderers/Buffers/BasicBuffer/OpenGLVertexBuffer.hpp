#pragma once

#include "VertexBuffer.hpp"

namespace Engine
{

class OpenGLVertexBuffer : public VertexBuffer
{
public:
    OpenGLVertexBuffer(size_t size);
    OpenGLVertexBuffer(const void *data);
    OpenGLVertexBuffer(const std::initializer_list<float> &vertices);
    virtual ~OpenGLVertexBuffer() override;

public:
    void Bind() const override;
    void Unbind() const override;
    void SetData(const void *data, size_t size, size_t offset) override;

protected:
    void CreateBuffer(const void *data, size_t size, BufferType type) override;
};

} // namespace Engine