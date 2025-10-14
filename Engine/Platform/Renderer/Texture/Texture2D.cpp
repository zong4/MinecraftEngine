#include "Texture2D.hpp"

#include <glad/glad.h>

MCEngine::Texture2D::Texture2D(int width, int height, unsigned int internalFormat, unsigned int format,
                               unsigned int type, void *data)
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

MCEngine::Texture2D::Texture2D(int width, int height, int samples)
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

MCEngine::Texture2D::Texture2D(const std::string &path) : Texture(), m_Type(GL_UNSIGNED_BYTE), m_Samples(0)
{
    ENGINE_PROFILE_FUNCTION();

    glGenTextures(1, &m_RendererID);
    glBindTexture(GL_TEXTURE_2D, m_RendererID);

    // Load image
    {
        unsigned char *data = LoadImage(path, m_Width, m_Height, m_InternalFormat, m_Format, true);
        glTexImage2D(GL_TEXTURE_2D, 0, m_InternalFormat, m_Width, m_Height, 0, m_Format, m_Type, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        RendererCommand::GetError(std::string(FUNCTION_SIGNATURE));
        FreeImage(data);
    }

    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    RendererCommand::GetError(std::string(FUNCTION_SIGNATURE));

    glBindTexture(GL_TEXTURE_2D, 0);
}

MCEngine::Texture2D::~Texture2D()
{
    ENGINE_PROFILE_FUNCTION();

    glDeleteTextures(1, &m_RendererID);
}

void MCEngine::Texture2D::Bind(unsigned int slot) const
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

void MCEngine::Texture2D::Unbind() const
{
    ENGINE_PROFILE_FUNCTION();

    glBindTexture(GL_TEXTURE_2D, 0);
    RendererCommand::GetError(std::string(FUNCTION_SIGNATURE));
}

void MCEngine::Texture2D::Resize(int width, int height)
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

void MCEngine::Texture2D::CreateTexture(int width, int height, unsigned int internalFormat, unsigned int format,
                                        unsigned int type, void *data)
{
    ENGINE_PROFILE_FUNCTION();

    glGenTextures(1, &m_RendererID);
    glBindTexture(GL_TEXTURE_2D, m_RendererID);

    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, type, data);
    RendererCommand::GetError(std::string(FUNCTION_SIGNATURE));
};