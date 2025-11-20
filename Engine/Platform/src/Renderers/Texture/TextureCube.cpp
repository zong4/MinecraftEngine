#include "TextureCube.hpp"

#include <glad/glad.h>

Engine::TextureCube::TextureCube(const glm::vec4 &color)
{
    PROFILE_FUNCTION();

    glGenTextures(1, &m_RendererID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererID);
    for (unsigned int i = 0; i < 6; i++)
    {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA8, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, &color);
    }
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

Engine::TextureCube::TextureCube(const std::array<std::string, 6> &faces) : Texture()
{
    PROFILE_FUNCTION();

    glGenTextures(1, &m_RendererID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererID);

    // Sampling parameters
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    RendererCommand::GetError(std::string(FUNCTION_SIGNATURE));

    int width = 0, height = 0, channels = 0;
    unsigned int internalFormat = 0, format = 0, type = 0;
    bool isHDR = false;

    // Record the first face's spec and validate the rest against it
    bool firstFace = true;
    int baseW = 0, baseH = 0;
    unsigned int baseIF = 0, baseFmt = 0, baseType = 0;
    bool baseIsHDR = false;

    for (unsigned int i = 0; i < faces.size(); ++i)
    {
        // Unified loader: returns void* (float* for HDR, unsigned char* for LDR)
        void *data = LoadImage(faces[i], width, height, channels, internalFormat, format, type, isHDR,
                               /*flipVertically=*/false);
        if (!data)
        {
            LOG_ENGINE_ASSERT("Failed to load cubemap face: " + faces[i]);
            continue;
        }

        if (firstFace)
        {
            // Lock the spec on the first face
            baseW = width;
            baseH = height;
            baseIF = internalFormat;
            baseFmt = format;
            baseType = type;
            baseIsHDR = isHDR;
            firstFace = false;

            // Set pixel alignment based on HDR/LDR: HDR (float) needs 4-byte alignment, LDR can use 1-byte
            glPixelStorei(GL_UNPACK_ALIGNMENT, baseIsHDR ? 4 : 1);
        }
        else
        {
            // Minimal validation (warn only; fix at asset level if mismatched)
            if (width != baseW || height != baseH)
            {
                LOG_ENGINE_WARN("Cubemap face size mismatch: " + faces[i]);
            }
            if (internalFormat != baseIF || format != baseFmt || type != baseType)
            {
                LOG_ENGINE_WARN("Cubemap face format/type mismatch: " + faces[i]);
            }
            if (isHDR != baseIsHDR)
            {
                LOG_ENGINE_WARN("Cubemap face HDR/LDR mismatch: " + faces[i] + " (expected " +
                                (baseIsHDR ? "HDR" : "LDR") + ")");
            }
        }

        // Upload using the first face's spec to keep the texture consistent
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, baseIF, width, height, 0, baseFmt, baseType, data);

        RendererCommand::GetError(std::string(FUNCTION_SIGNATURE));

        // Works for both stbi_load (U8) and stbi_loadf (float)
        FreeImage(data);
    }

    // glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    LOG_ENGINE_TRACE("TextureCube created with RendererID: " + std::to_string((uint32_t)m_RendererID));
}

Engine::TextureCube::~TextureCube() { glDeleteTextures(1, &m_RendererID); }

std::shared_ptr<Engine::TextureCube> Engine::TextureCube::WhiteTexture()
{
    PROFILE_FUNCTION();

    static std::shared_ptr<TextureCube> whiteTexture = std::make_shared<TextureCube>(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
    return whiteTexture;
}

void Engine::TextureCube::Bind(unsigned int slot) const
{
    PROFILE_FUNCTION();

    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererID);
    RendererCommand::GetError(std::string(FUNCTION_SIGNATURE));
}

void Engine::TextureCube::Unbind() const
{
    PROFILE_FUNCTION();

    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    RendererCommand::GetError(std::string(FUNCTION_SIGNATURE));
}