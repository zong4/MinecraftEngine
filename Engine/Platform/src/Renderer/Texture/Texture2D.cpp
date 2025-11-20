#include "Texture2D.hpp"

#include <glad/glad.h>

Engine::Texture2D::Texture2D(int width, int height, unsigned int internalFormat, unsigned int format, unsigned int type,
                             void *data)
    : Texture(), m_Width(width), m_Height(height), m_InternalFormat(internalFormat), m_Format(format), m_Type(type),
      m_Samples(0)
{
    ENGINE_PROFILE_FUNCTION();

    CreateTexture(width, height, internalFormat, format, type, data);

    // Set texture parameters
    if (m_Format == GL_DEPTH_COMPONENT)
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        float borderColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    }
    else if (m_Format == GL_RGBA)
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    RendererCommand::GetError(std::string(FUNCTION_SIGNATURE));

    glBindTexture(GL_TEXTURE_2D, 0);
}

Engine::Texture2D::Texture2D(int width, int height, int samples)
    : Texture(), m_Width(width), m_Height(height), m_InternalFormat(GL_RGBA8), m_Format(GL_RGBA),
      m_Type(GL_UNSIGNED_BYTE), m_Samples(samples)
{
    ENGINE_PROFILE_FUNCTION();

    glGenTextures(1, &m_RendererID);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_RendererID);

    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, m_InternalFormat, width, height, GL_TRUE);
    RendererCommand::GetError(std::string(FUNCTION_SIGNATURE));

    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
}

Engine::Texture2D::Texture2D(const std::string &path) : Texture(), m_Type(GL_UNSIGNED_BYTE), m_Samples(0)
{
    ENGINE_PROFILE_FUNCTION();

    glGenTextures(1, &m_RendererID);
    glBindTexture(GL_TEXTURE_2D, m_RendererID);

    // Load image
    bool isHDR = false;
    {
        int channels = 0;
        unsigned int type = 0;
        void *data =
            LoadImage(path, m_Width, m_Height, channels, m_InternalFormat, m_Format, type, isHDR, /*flip*/ true);

        m_Type = type;

        // Set pixel alignment: HDR textures (float) need 4-byte alignment, LDR can use 1-byte
        glPixelStorei(GL_UNPACK_ALIGNMENT, isHDR ? 4 : 1);
        glTexImage2D(GL_TEXTURE_2D, 0, m_InternalFormat, m_Width, m_Height, 0, m_Format, m_Type, data);

        // Generate mipmap only for LDR textures
        // HDR textures (especially environment maps) typically don't need mipmaps
        // and some OpenGL implementations may not support mipmap generation for float textures
        if (!isHDR)
        {
            glGenerateMipmap(GL_TEXTURE_2D);
        }

        RendererCommand::GetError(std::string(FUNCTION_SIGNATURE));
        FreeImage(data);
    }

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
        // LDR textures: use mipmap filtering
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    }
    RendererCommand::GetError(std::string(FUNCTION_SIGNATURE));

    glBindTexture(GL_TEXTURE_2D, 0);
}

Engine::Texture2D::~Texture2D()
{
    ENGINE_PROFILE_FUNCTION();

    glDeleteTextures(1, &m_RendererID);
}

std::shared_ptr<Engine::Texture2D> Engine::Texture2D::WhiteTexture()
{
    ENGINE_PROFILE_FUNCTION();

    static std::shared_ptr<Texture2D> whiteTexture = std::make_shared<Texture2D>(
        1, 1, GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE, new unsigned char[4]{255, 255, 255, 255});
    return whiteTexture;
}

void Engine::Texture2D::Bind(unsigned int slot) const
{
    ENGINE_PROFILE_FUNCTION();

    if (m_Samples > 0)
    {
        LOG_ENGINE_WARN("Trying to bind a multisampled texture to slot: " + std::to_string(slot));
        return;
    }

    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, m_RendererID);
    RendererCommand::GetError(std::string(FUNCTION_SIGNATURE));
}

void Engine::Texture2D::Unbind() const
{
    ENGINE_PROFILE_FUNCTION();

    glBindTexture(GL_TEXTURE_2D, 0);
    RendererCommand::GetError(std::string(FUNCTION_SIGNATURE));
}

void Engine::Texture2D::Resize(int width, int height)
{
    ENGINE_PROFILE_FUNCTION();

    m_Width = width;
    m_Height = height;

    if (m_Samples == 0)
    {
        glBindTexture(GL_TEXTURE_2D, m_RendererID);
        glTexImage2D(GL_TEXTURE_2D, 0, m_InternalFormat, m_Width, m_Height, 0, m_Format, m_Type, nullptr);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    else
    {
        glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_RendererID);
        glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, m_Samples, m_InternalFormat, m_Width, m_Height, GL_TRUE);
        glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
    }
    RendererCommand::GetError(std::string(FUNCTION_SIGNATURE));
}

void Engine::Texture2D::CreateTexture(int width, int height, unsigned int internalFormat, unsigned int format,
                                      unsigned int type, void *data)
{
    ENGINE_PROFILE_FUNCTION();

    glGenTextures(1, &m_RendererID);
    glBindTexture(GL_TEXTURE_2D, m_RendererID);

    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, type, data);
    RendererCommand::GetError(std::string(FUNCTION_SIGNATURE));
};