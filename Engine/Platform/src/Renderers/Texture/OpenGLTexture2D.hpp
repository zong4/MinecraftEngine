#pragma once

#include "Texture2D.hpp"

namespace Engine
{

class OpenGLTexture2D : public Texture2D
{
public:
    // OpenGLTexture2D(int width, int height, void *data);
    OpenGLTexture2D(int width, int height, unsigned int internalFormat, unsigned int format, unsigned int type,
                    void *data = nullptr);
    OpenGLTexture2D(int width, int height, int samples);
    OpenGLTexture2D(const std::string &path);
    virtual ~OpenGLTexture2D() override;

public:
    void Bind(unsigned int slot) const override;
    void Unbind() const override;
    void Resize(int width, int height) override;

protected:
    void CreateTexture(int width, int height, unsigned int internalFormat, unsigned int format, unsigned int type,
                       void *data);
};

} // namespace Engine