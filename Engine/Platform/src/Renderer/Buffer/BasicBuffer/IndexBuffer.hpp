#pragma once

#include "BasicBuffer.hpp"

namespace MCEngine
{

class IndexBuffer : public BasicBuffer
{
public:
    // IndexBuffer() = default;
    IndexBuffer(size_t size);
    IndexBuffer(const void *data, size_t size);
    IndexBuffer(const std::vector<uint32_t> &indices);
    virtual ~IndexBuffer() override;

    IndexBuffer(IndexBuffer &&other);
    IndexBuffer &operator=(IndexBuffer &&other);

public:
    void Bind() const override;
    void Unbind() const override;

    void SetData(const void *data, size_t size, size_t offset) override;

protected:
    void CreateBuffer(const void *data, size_t size) override;
};

} // namespace MCEngine
