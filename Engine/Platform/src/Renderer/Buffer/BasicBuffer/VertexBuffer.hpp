#pragma once

#include "BasicBuffer.hpp"

namespace MCEngine
{

class VertexBuffer : public BasicBuffer
{
public:
    VertexBuffer(size_t size);
    VertexBuffer(const void *data, size_t size);
    VertexBuffer(const std::vector<float> &vertices);
    virtual ~VertexBuffer() override;

    VertexBuffer(VertexBuffer &&other);
    VertexBuffer &operator=(VertexBuffer &&other);

public:
    void Bind() const override;
    void Unbind() const override;

    void SetData(const void *data, size_t size, size_t offset) override;

protected:
    void CreateBuffer(const void *data, size_t size) override;
};

} // namespace MCEngine