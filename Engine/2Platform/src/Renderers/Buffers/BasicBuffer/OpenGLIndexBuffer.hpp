#pragma once

#include "IndexBuffer.hpp"

namespace Engine
{

class OpenGLIndexBuffer : public IndexBuffer
{
public:
    OpenGLIndexBuffer(size_t size);
    OpenGLIndexBuffer(const void *data);
    OpenGLIndexBuffer(const std::initializer_list<uint32_t> &indices);
    virtual ~OpenGLIndexBuffer() override;

public:
    void Bind() const override;
    void Unbind() const override;
    void SetData(const void *data, size_t size, size_t offset) override;

protected:
    void CreateBuffer(const void *data, size_t size, BufferType type) override;
};

} // namespace Engine
