#pragma once

#include "../Texture/Texture2D.hpp"
#include "RenderBuffer.hpp"

namespace Engine
{

enum class FrameBufferType
{
    Color,
    Depth,
    Integer,
    Multisample,
};

class FrameBuffer
{
public:
    virtual ~FrameBuffer() = default;
    static std::shared_ptr<FrameBuffer> Create(FrameBufferType type, int width, int height, int samples = 0);

    // Getters
    unsigned int GetRendererID() const { return m_RendererID; }
    int GetWidth() const { return m_Width; }
    int GetHeight() const { return m_Height; }
    std::shared_ptr<Texture2D> GetTexture() const { return m_Texture; }

public:
    virtual void Bind() const = 0;
    virtual void Blit(unsigned int resolveID) const = 0;
    virtual void Unbind() const = 0;
    virtual unsigned int PickPixel(int x, int y) const = 0;
    void Resize(int width, int height);

protected:
    FrameBufferType m_Type;
    unsigned int m_RendererID = 0;
    int m_Width, m_Height;
    std::shared_ptr<Texture2D> m_Texture = nullptr;
    std::unique_ptr<RenderBuffer> m_RenderBuffer = nullptr;

protected:
    FrameBuffer(FrameBufferType type, int width, int height, int samples)
        : m_Type(type), m_Width(width), m_Height(height)
    {
    }

protected:
    virtual void BindBasicTexture(int width, int height) = 0;
    virtual void BindMultiSampleTexture(int width, int height, int samples) = 0;
    virtual void BindRenderBuffer(int width, int height, unsigned int internalFormat, int samples) = 0;
};

} // namespace Engine