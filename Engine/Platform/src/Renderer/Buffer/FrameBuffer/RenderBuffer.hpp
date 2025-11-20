#pragma once

#include "Renderer/RendererCommand.hpp"

namespace MCEngine
{

class RenderBuffer
{
public:
    RenderBuffer(int width, int height, unsigned int internalFormat, int samples);
    ~RenderBuffer();

    RenderBuffer(const RenderBuffer &) = delete;
    RenderBuffer &operator=(const RenderBuffer &) = delete;
    RenderBuffer(RenderBuffer &&) = delete;
    RenderBuffer &operator=(RenderBuffer &&) = delete;

    // Getters
    unsigned int GetRendererID() const { return m_RendererID; }

public:
    void Resize(int width, int height);

private:
    unsigned int m_RendererID = 0;
    unsigned int m_InternalFormat;
    int m_Samples;

private:
    void Bind(int width, int height) const;
    void Unbind() const;
};

} // namespace MCEngine