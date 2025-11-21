#pragma once

#include "BasicBuffer.hpp"

namespace Engine
{

class VertexBuffer : public BasicBuffer
{
public:
    virtual ~VertexBuffer() override = default;
    static std::unique_ptr<VertexBuffer> Create(size_t size);
    static std::unique_ptr<VertexBuffer> Create(const void *data);
    static std::unique_ptr<VertexBuffer> Create(const std::initializer_list<float> &vertices);

protected:
    VertexBuffer(int count) : BasicBuffer(count) {}
};

} // namespace Engine