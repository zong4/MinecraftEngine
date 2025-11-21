#pragma once

#include <Core.hpp>

namespace Engine
{

struct UniformBufferData
{
    const void *Data;
    size_t Size;
    size_t Offset;
};

class UniformBuffer
{
public:
    virtual ~UniformBuffer() = default;
    static std::shared_ptr<UniformBuffer> Create(size_t size, unsigned int binding);

    // Getters
    unsigned int GetRendererID() const { return m_RendererID; }

public:
    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;
    virtual void SetData(const std::initializer_list<UniformBufferData> &dataList) = 0;

protected:
    unsigned int m_RendererID = 0;
    unsigned int m_Binding = 0;

protected:
    UniformBuffer(size_t size, unsigned int binding) : m_Binding(binding) {}
};

} // namespace Engine