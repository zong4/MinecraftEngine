#pragma once

#include "Texture.hpp"

namespace MCEngine
{

class Texture2D : public Texture
{
public:
    // Texture2D(int width, int height, void *data);
    Texture2D(int width, int height, unsigned int internalFormat, unsigned int format, unsigned int type,
              void *data = nullptr);
    Texture2D(int width, int height, int samples);
    Texture2D(const std::string &path);
    virtual ~Texture2D() override;

    // Getters
    int GetWidth() const { return m_Width; }
    int GetHeight() const { return m_Height; }

public:
    virtual void Bind(unsigned int slot) const override;
    virtual void Unbind() const override;

    void Resize(int width, int height);

private:
    int m_Width, m_Height;
    unsigned int m_InternalFormat, m_Format, m_Type;
    int m_Samples;

private:
    void CreateTexture(int width, int height, unsigned int internalFormat, unsigned int format, unsigned int type,
                       void *data);
};

} // namespace MCEngine