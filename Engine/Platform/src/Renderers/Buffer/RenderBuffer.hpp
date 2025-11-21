#pragma once

#include <Core.hpp>

namespace Engine
{

class RenderBuffer
{
public:
    virtual ~RenderBuffer() = default;
    static std::unique_ptr<RenderBuffer> Create(int width, int height, unsigned int internalFormat, int samples);

    // Getters
    unsigned int GetRendererID() const { return m_RendererID; }

public:
    void Resize(int width, int height);

protected:
    unsigned int m_RendererID = 0;
    unsigned int m_InternalFormat;
    int m_Samples;

protected:
    RenderBuffer(int width, int height, unsigned int internalFormat, int samples)
        : m_InternalFormat(internalFormat), m_Samples(samples)
    {
    }

protected:
    virtual void Bind(int width, int height) const = 0;
    virtual void Unbind() const = 0;
};

} // namespace Engine