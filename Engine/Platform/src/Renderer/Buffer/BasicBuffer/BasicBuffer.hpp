#pragma once

#include "Renderer/RendererCommand.hpp"

namespace MCEngine
{

class BasicBuffer
{
public:
    BasicBuffer(int count = 0) : m_Count(count) {}
    virtual ~BasicBuffer() = default;
    BasicBuffer(const BasicBuffer &) = delete;
    BasicBuffer &operator=(const BasicBuffer &) = delete;

    // Getters
    unsigned int GetRendererID() const { return m_RendererID; }
    int GetCount() const { return m_Count; }

public:
    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;

    virtual void SetData(const void *data, size_t size, size_t offset) = 0;

protected:
    unsigned int m_RendererID = 0;
    int m_Count;

protected:
    virtual void CreateBuffer(const void *data, size_t size) = 0;
};

} // namespace MCEngine