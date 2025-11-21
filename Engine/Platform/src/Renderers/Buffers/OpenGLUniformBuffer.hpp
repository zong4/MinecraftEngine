#pragma once

#include "UniformBuffer.hpp"

namespace Engine
{

class OpenGLUniformBuffer : public UniformBuffer
{
public:
    OpenGLUniformBuffer(size_t size, unsigned int binding);
    virtual ~OpenGLUniformBuffer() override;

    // Getters
    unsigned int GetRendererID() const { return m_RendererID; }

public:
    void Bind() const override;
    void Unbind() const override;
    void SetData(const std::initializer_list<UniformBufferData> &dataList) override;
};

} // namespace Engine