#pragma once

#include "Buffer.hpp"

namespace Engine
{

class IndexBuffer : public Buffer
{
public:
    virtual ~IndexBuffer() override = default;
    static std::unique_ptr<IndexBuffer> Create(size_t size); // size in bytes
    static std::unique_ptr<IndexBuffer> Create(const std::initializer_list<uint32_t> &indices);

protected:
    // IndexBuffer() = default;
    IndexBuffer(int count) : Buffer(count) {}
};

} // namespace Engine
