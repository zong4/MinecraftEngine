#pragma once

#include "FrameBuffer.hpp"

namespace Engine
{

class OpenGLFrameBuffer : public FrameBuffer
{
public:
    OpenGLFrameBuffer(Texture2DType type, int width, int height, int samples = 0);
    virtual ~OpenGLFrameBuffer() override;

public:
    void Bind() const override;
    void Blit(unsigned int resolveID) const override;
    void Unbind() const override;
    unsigned int PickPixel(int x, int y) const override;

protected:
    void BindBasicTexture(Texture2DType type, int width, int height) override;
    void BindMultiSampleTexture(int width, int height, int samples) override;
    void BindRenderBuffer(int width, int height, unsigned int internalFormat, int samples) override;
};

} // namespace Engine