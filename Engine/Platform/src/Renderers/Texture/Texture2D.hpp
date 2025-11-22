#pragma once

#include "Texture.hpp"

namespace Engine
{

enum class Texture2DType
{
    Integer,
    Color,
    Depth,
    Multisample,
};

class Texture2D : public Texture
{
public:
    virtual ~Texture2D() override = default;
    static std::shared_ptr<Texture2D> Create(Texture2DType type, int width, int height, void *data = nullptr);
    static std::shared_ptr<Texture2D> Create(int width, int height, int samples);
    static std::shared_ptr<Texture2D> Create(const std::string &path);
    static std::shared_ptr<Texture2D> WhiteTexture();

    // Getters
    Texture2DType GetType() const { return m_Type; }
    int GetWidth() const { return m_Width; }
    int GetHeight() const { return m_Height; }

public:
    virtual void Resize(int width, int height) = 0;

protected:
    Texture2DType m_Type;
    int m_Width, m_Height;
    int m_Samples;

protected:
    Texture2D() = default;
    Texture2D(Texture2DType type, int width, int height, void *data)
        : Texture(), m_Type(type), m_Width(width), m_Height(height), m_Samples(0)
    {
    }
    Texture2D(int width, int height, int samples)
        : Texture(), m_Type(Texture2DType::Multisample), m_Width(width), m_Height(height), m_Samples(samples)
    {
    }

protected:
    virtual void CreateTexture(int width, int height, unsigned int internalFormat, unsigned int format,
                               unsigned int type, void *data, bool isHDR) = 0;
};

} // namespace Engine