#include "Texture.hpp"

#include <glad/glad.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

// unsigned char *Engine::Texture::LoadImage(const std::string &path, int &width, int &height,
//                                             unsigned int &internalFormat, unsigned int &format, bool flipVertically)
// {
//     ENGINE_PROFILE_FUNCTION();

//     // Check if file exists
//     if (!std::filesystem::exists(path))
//     {
//         LOG_ENGINE_WARN("Texture file does not exist: " + path);
//         return nullptr;
//     }

//     // Load image
//     int channels;
//     stbi_set_flip_vertically_on_load(flipVertically);
//     unsigned char *data = stbi_load(path.c_str(), &width, &height, &channels, 0);

//     if (channels == 1)
//     {
//         internalFormat = GL_R8;
//         format = GL_RED;
//     }
//     else if (channels == 3)
//     {
//         internalFormat = GL_RGB8;
//         format = GL_RGB;
//     }
//     else if (channels == 4)
//     {
//         internalFormat = GL_RGBA8;
//         format = GL_RGBA;
//     }
//     else
//     {
//         LOG_ENGINE_ASSERT("Unsupported number of channels: " + std::to_string(channels) + " in texture: " + path);
//         stbi_image_free(data);
//         return nullptr;
//     }

//     if (!data)
//     {
//         LOG_ENGINE_ASSERT("Failed to load texture: " + path);
//         return nullptr;
//     }
//     return data;
// }

static inline std::string LowerExt(const std::string &p)
{
    auto e = std::filesystem::path(p).extension().string();
    std::transform(e.begin(), e.end(), e.begin(), ::tolower);
    return e;
}

void *Engine::Texture::LoadImage(const std::string &path, int &w, int &h, int &ch, unsigned int &internalFormat,
                                 unsigned int &format, unsigned int &type, bool &isHDR, bool flip)
{
    if (!std::filesystem::exists(path))
        throw std::runtime_error("Texture file does not exist: " + path);

    stbi_set_flip_vertically_on_load(flip);

    const std::string ext = LowerExt(path);
    isHDR = (ext == ".hdr") || stbi_is_hdr(path.c_str());

    void *data = nullptr;

    if (isHDR)
    {
        float *f = stbi_loadf(path.c_str(), &w, &h, &ch, 0);
        if (!f)
            throw std::runtime_error("Failed to load HDR: " + path);
        data = (void *)f;
        type = GL_FLOAT;
    }
    else
    {
        unsigned char *u = stbi_load(path.c_str(), &w, &h, &ch, 0);
        if (!u)
            throw std::runtime_error("Failed to load LDR: " + path);
        data = (void *)u;
        type = GL_UNSIGNED_BYTE;
    }

    switch (ch)
    {
    case 1:
        format = GL_RED;
        break;
    case 2:
        format = GL_RG;
        break;
    case 3:
        format = GL_RGB;
        break;
    case 4:
        format = GL_RGBA;
        break;
    default: {
        LOG_ENGINE_ASSERT("Unsupported number of channels: " + std::to_string(ch) + " in texture: " + path);
        stbi_image_free(data);
        return nullptr;
    }
    }

    if (isHDR)
    {
        if (ch == 1)
            internalFormat = GL_R16F;
        else if (ch == 2)
            internalFormat = GL_RG16F;
        else if (ch == 3)
            internalFormat = GL_RGB16F;
        else if (ch == 4)
            internalFormat = GL_RGBA16F;
    }
    else
    {
        if (ch == 1)
            internalFormat = GL_R8;
        else if (ch == 2)
            internalFormat = GL_RG8;
        else if (ch == 3)
            internalFormat = GL_SRGB8;
        else if (ch == 4)
            internalFormat = GL_SRGB8_ALPHA8;
    }

    return data;
}

void Engine::Texture::FreeImage(void *data)
{
    ENGINE_PROFILE_FUNCTION();

    stbi_image_free(data);
}
