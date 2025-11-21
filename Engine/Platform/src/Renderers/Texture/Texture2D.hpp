#pragma once

#include "Texture.hpp"

namespace Engine
{

class Texture2D : public Texture
{
public:
    static std::shared_ptr<Texture2D> Create(int width, int height, unsigned int internalFormat, unsigned int format,
                                             unsigned int type, void *data = nullptr);
    static std::shared_ptr<Texture2D> Create(int width, int height, int samples);
    static std::shared_ptr<Texture2D> Create(const std::string &path);
    static std::shared_ptr<Texture2D> WhiteTexture();

    // Getters
    int GetWidth() const { return m_Width; }
    int GetHeight() const { return m_Height; }

public:
    virtual void Resize(int width, int height) = 0;

protected:
    int m_Width, m_Height;
    unsigned int m_InternalFormat, m_Format, m_Type;
    int m_Samples;

protected:
    Texture2D() = default;
    Texture2D(int width, int height, unsigned int internalFormat, unsigned int format, unsigned int type, int samples)
        : Texture(), m_Width(width), m_Height(height), m_InternalFormat(internalFormat), m_Format(format), m_Type(type),
          m_Samples(samples)
    {
    }
    virtual ~Texture2D() override = default;
};

} // namespace Engine