#include "Texture.hpp"

#include <glad/glad.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

unsigned char *MCEngine::Texture::LoadImage(const std::string &path, int &width, int &height,
                                            unsigned int &internalFormat, unsigned int &format, bool flipVertically)
{
    ENGINE_PROFILE_FUNCTION();

    // Check if file exists
    if (!std::filesystem::exists(path))
    {
        LOG_ENGINE_WARN("Texture file does not exist: " + path);
        return nullptr;
    }

    // Load image
    int channels;
    stbi_set_flip_vertically_on_load(flipVertically);
    unsigned char *data = stbi_load(path.c_str(), &width, &height, &channels, 0);

    if (channels == 1)
    {
        internalFormat = GL_R8;
        format = GL_RED;
    }
    else if (channels == 3)
    {
        internalFormat = GL_RGB8;
        format = GL_RGB;
    }
    else if (channels == 4)
    {
        internalFormat = GL_RGBA8;
        format = GL_RGBA;
    }
    else
    {
        LOG_ENGINE_ERROR("Unsupported number of channels: " + std::to_string(channels) + " in texture: " + path);
        stbi_image_free(data);
        return nullptr;
    }

    if (!data)
    {
        LOG_ENGINE_ERROR("Failed to load texture: " + path);
        return nullptr;
    }
    return data;
}

void MCEngine::Texture::FreeImage(unsigned char *data)
{
    ENGINE_PROFILE_FUNCTION();

    stbi_image_free(data);
}
