#pragma once

#include "Renderer/RendererCommand.hpp"

namespace MCEngine
{

struct UniformBufferData
{
    const void *Data;
    size_t Size;
    size_t Offset;

public:
    UniformBufferData(const void *data, size_t size, size_t offset) : Data(data), Size(size), Offset(offset) {}
};

class UniformBuffer
{
public:
    UniformBuffer(size_t size, unsigned int binding);
    ~UniformBuffer();

    UniformBuffer(const UniformBuffer &) = delete;
    UniformBuffer &operator=(const UniformBuffer &) = delete;
    UniformBuffer(UniformBuffer &&) = delete;
    UniformBuffer &operator=(UniformBuffer &&) = delete;

    // Getters
    unsigned int GetRendererID() const { return m_RendererID; }

public:
    void Bind() const;
    void Unbind() const;

    void SetData(const std::initializer_list<UniformBufferData> &dataList);

private:
    unsigned int m_RendererID = 0;
    unsigned int m_Binding = 0;
};

} // namespace MCEngine