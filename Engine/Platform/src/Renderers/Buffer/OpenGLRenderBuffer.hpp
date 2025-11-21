#pragma once

#include "RenderBuffer.hpp"

namespace Engine
{

class OpenGLRenderBuffer : public RenderBuffer
{
public:
    OpenGLRenderBuffer(int width, int height, unsigned int internalFormat, int samples);
    virtual ~OpenGLRenderBuffer() override;

protected:
    void Bind(int width, int height) const override;
    void Unbind() const override;
};

} // namespace Engine