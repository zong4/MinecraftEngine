#include "TextureCube.hpp"

#include <glad/glad.h>

MCEngine::TextureCube::TextureCube(const glm::vec4 &color)
{
    ENGINE_PROFILE_FUNCTION();

    glGenTextures(1, &m_RendererID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererID);
    for (unsigned int i = 0; i < 6; i++)
    {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA8, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, &color);
    }
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

MCEngine::TextureCube::TextureCube(const std::array<std::string, 6> &faces) : Texture()
{
    // std::array<std::string, 6> faces = {directory + "/right.jpg",  directory + "/left.jpg",  directory + "/top.jpg",
    //                                     directory + "/bottom.jpg", directory + "/front.jpg", directory +
    //                                     "/back.jpg"};

    ENGINE_PROFILE_FUNCTION();

    glGenTextures(1, &m_RendererID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererID);

    // Set texture parameters
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    RendererCommand::GetError(std::string(FUNCTION_SIGNATURE));

    int width, height;
    unsigned int internalFormat, format;
    unsigned char *data = nullptr;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        data = LoadImage(faces[i], width, height, internalFormat, format, false);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE,
                     data);
        RendererCommand::GetError(std::string(FUNCTION_SIGNATURE));

        FreeImage(data);
    }
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    LOG_ENGINE_TRACE("TextureCube created with RendererID: " + std::to_string((uint32_t)m_RendererID));
}

MCEngine::TextureCube::~TextureCube() { glDeleteTextures(1, &m_RendererID); }

std::shared_ptr<MCEngine::TextureCube> MCEngine::TextureCube::WhiteTexture()
{
    ENGINE_PROFILE_FUNCTION();

    static std::shared_ptr<TextureCube> whiteTexture = std::make_shared<TextureCube>(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
    return whiteTexture;
}

void MCEngine::TextureCube::Bind(unsigned int slot) const
{
    ENGINE_PROFILE_FUNCTION();

    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererID);
    RendererCommand::GetError(std::string(FUNCTION_SIGNATURE));
}

void MCEngine::TextureCube::Unbind() const
{
    ENGINE_PROFILE_FUNCTION();

    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    RendererCommand::GetError(std::string(FUNCTION_SIGNATURE));
}