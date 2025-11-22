#include "OpenGLTextureCube.hpp"

#include "../RendererCommand.hpp"
#include <glad/glad.h>

Engine::OpenGLTextureCube::OpenGLTextureCube(const glm::vec4 &color) : TextureCube()
{
    glGenTextures(1, &m_RendererID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererID);
    for (unsigned int i = 0; i < 6; i++)
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA8, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, &color);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

Engine::OpenGLTextureCube::OpenGLTextureCube(const std::array<std::string, 6> &faces) : TextureCube()
{
    glGenTextures(1, &m_RendererID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererID);

    // Record the first face's spec and validate the rest against it
    bool baseIsHDR = false;
    int baseWidth = 0, baseHeight = 0;
    unsigned int baseInternalFormat = 0, baseFormat = 0, baseType = 0;

    bool isHDR = false;
    int width = 0, height = 0, channels = 0;
    unsigned int internalFormat = 0, format = 0, type = 0;
    for (int i = 0; i < faces.size(); i++)
    {
        void *data = LoadImage(faces[i], width, height, channels, isHDR, false);
        type = isHDR ? GL_FLOAT : GL_UNSIGNED_BYTE;
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

        if (i == 0)
        {
            // Lock the spec on the first face
            baseIsHDR = isHDR;
            baseWidth = width;
            baseHeight = height;
            baseInternalFormat = internalFormat;
            baseFormat = format;
            baseType = type;

            // Set pixel alignment based on HDR/LDR: HDR (float) needs 4-byte alignment, LDR can use 1-byte
            glPixelStorei(GL_UNPACK_ALIGNMENT, baseIsHDR ? 4 : 1);
        }
        else
        {
            // Minimal validation (warn only; fix at asset level if mismatched)
            if (isHDR != baseIsHDR)
                LOG_ENGINE_ERROR("Cubemap face HDR/LDR mismatch: " + faces[i] + " (expected " +
                                 (baseIsHDR ? "HDR" : "LDR") + ")");
            if (width != baseWidth || height != baseHeight)
                LOG_ENGINE_ERROR("Cubemap face size mismatch: " + faces[i]);
            if (internalFormat != baseInternalFormat || format != baseFormat || type != baseType)
                LOG_ENGINE_ERROR("Cubemap face format/type mismatch: " + faces[i]);
        }

        // Upload using the first face's spec to keep the texture consistent
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, baseInternalFormat, width, height, 0, baseFormat, baseType,
                     data);
        RendererCommand::GetError(std::string(FUNCTION_SIGNATURE));
        FreeImage(data);
    }

    // Set texture parameters
    if (baseIsHDR)
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    else
    {
        glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
        RendererCommand::GetError(std::string(FUNCTION_SIGNATURE));
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    Unbind();
    LOG_ENGINE_TRACE("OpenGLTextureCube created with RendererID: " + std::to_string((uint32_t)m_RendererID));
}

Engine::OpenGLTextureCube::~OpenGLTextureCube() { glDeleteTextures(1, &m_RendererID); }

void Engine::OpenGLTextureCube::Bind() const { glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererID); }

void Engine::OpenGLTextureCube::Unbind() const { glBindTexture(GL_TEXTURE_CUBE_MAP, 0); }

void Engine::OpenGLTextureCube::Active(unsigned int slot) const
{
    glActiveTexture(GL_TEXTURE0 + slot);
    Bind();
}