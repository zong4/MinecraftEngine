#include "OpenGLTexture2D.hpp"

#include "../RendererCommand.hpp"
#include <glad/glad.h>

Engine::OpenGLTexture2D::OpenGLTexture2D(Texture2DType type, int width, int height, void *data)
    : Texture2D(type, width, height, data)
{
    PROFILE_FUNCTION();

    switch (m_Type)
    {
    case Texture2DType::Integer:
        CreateTexture(width, height, GL_R32UI, GL_RED_INTEGER, GL_UNSIGNED_INT, data);
        break;
    case Texture2DType::Color:
        CreateTexture(width, height, GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        break;
    case Texture2DType::Depth: {
        CreateTexture(width, height, GL_DEPTH_COMPONENT24, GL_DEPTH_COMPONENT, GL_FLOAT, data);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        GLfloat borderColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
        break;
    }
    default:
        LOG_ENGINE_ASSERT("Unsupported Texture2DType for this constructor");
        break;
    }
    RendererCommand::GetError(std::string(FUNCTION_SIGNATURE));
    Unbind();
}

Engine::OpenGLTexture2D::OpenGLTexture2D(int width, int height, int samples) : Texture2D(width, height, samples)
{
    PROFILE_FUNCTION();

    glGenTextures(1, &m_RendererID);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_RendererID);
    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, GL_RGBA8, width, height, GL_TRUE);
    RendererCommand::GetError(std::string(FUNCTION_SIGNATURE));
    Unbind();
}

Engine::OpenGLTexture2D::OpenGLTexture2D(const std::string &path) : Texture2D()
{
    PROFILE_FUNCTION();

    // Load image
    int channels = 0;
    bool isHDR = false;
    void *data = LoadImage(path, m_Width, m_Height, channels, isHDR, /*flip*/ true);

    // Determine format and internal format
    unsigned int format, internalFormat = 0;
    switch (channels)
    {
    case 1:
        format = GL_RED;
        internalFormat = isHDR ? GL_R16F : GL_R8;
        break;
    case 2:
        format = GL_RG;
        internalFormat = isHDR ? GL_RG16F : GL_RG8;
        break;
    case 3:
        format = GL_RGB;
        internalFormat = isHDR ? GL_RGB16F : GL_SRGB8;
        break;
    case 4:
        format = GL_RGBA;
        internalFormat = isHDR ? GL_RGBA16F : GL_SRGB8_ALPHA8;
        break;
    default:
        LOG_ENGINE_ASSERT("Unsupported number of channels in texture: " + std::to_string(channels));
        break;
    }
    CreateTexture(m_Width, m_Height, internalFormat, format, isHDR ? GL_FLOAT : GL_UNSIGNED_BYTE, data, isHDR);
    FreeImage(data);

    // Set texture parameters based on HDR/LDR
    if (isHDR)
    {
        // HDR textures: use linear filtering without mipmap
        // For environment maps, use CLAMP_TO_EDGE to avoid seams
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }
    else
    {
        // Generate mipmap only for LDR textures
        // HDR textures (especially environment maps) typically don't need mipmaps
        // and some OpenGL implementations may not support mipmap generation for float textures
        // LDR textures: use mipmap filtering
        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    }
    RendererCommand::GetError(std::string(FUNCTION_SIGNATURE));
    Unbind();
}

Engine::OpenGLTexture2D::~OpenGLTexture2D() { glDeleteTextures(1, &m_RendererID); }

void Engine::OpenGLTexture2D::Bind() const
{
    switch (m_Type)
    {
    case Texture2DType::Multisample:
        glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_RendererID);
        break;
    default:
        glBindTexture(GL_TEXTURE_2D, m_RendererID);
        break;
    }
}

void Engine::OpenGLTexture2D::Unbind() const
{
    switch (m_Type)
    {
    case Texture2DType::Multisample:
        glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
        break;
    default:
        glBindTexture(GL_TEXTURE_2D, 0);
        break;
    }
}

void Engine::OpenGLTexture2D::Active(unsigned int slot) const
{
    if (m_Type == Texture2DType::Multisample)
    {
        LOG_ENGINE_WARN("Activating multisample textures is not supported.");
        return;
    }
    glActiveTexture(GL_TEXTURE0 + slot);
    Bind();
}

void Engine::OpenGLTexture2D::Resize(int width, int height)
{
    PROFILE_FUNCTION();

    if (width == m_Width && height == m_Height)
        return;
    m_Width = width;
    m_Height = height;

    Bind();
    switch (m_Type)
    {
    case Texture2DType::Color:
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        break;
    case Texture2DType::Depth:
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, m_Width, m_Height, 0, GL_DEPTH_COMPONENT, GL_FLOAT,
                     nullptr);
        break;
    case Texture2DType::Integer:
        glTexImage2D(GL_TEXTURE_2D, 0, GL_R32UI, m_Width, m_Height, 0, GL_RED_INTEGER, GL_UNSIGNED_INT, nullptr);
        break;
    case Texture2DType::Multisample:
        glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, m_Samples, GL_RGBA8, m_Width, m_Height, GL_TRUE);
        break;
    default:
        LOG_ENGINE_ASSERT("Unsupported Texture2DType for Resize.");
        break;
    }
    RendererCommand::GetError(std::string(FUNCTION_SIGNATURE));
    Unbind();
}

void Engine::OpenGLTexture2D::CreateTexture(int width, int height, unsigned int internalFormat, unsigned int format,
                                            unsigned int type, void *data, bool isHDR)
{
    PROFILE_FUNCTION();

    glGenTextures(1, &m_RendererID);
    glBindTexture(GL_TEXTURE_2D, m_RendererID);
    glPixelStorei(GL_UNPACK_ALIGNMENT, isHDR ? 4 : 1);
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, type, data);
    RendererCommand::GetError(std::string(FUNCTION_SIGNATURE));
};