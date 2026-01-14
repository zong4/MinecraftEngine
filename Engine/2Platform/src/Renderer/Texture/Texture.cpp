#include "Texture.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

void Engine::Texture::SaveImage(const std::string &path, int width, int height, const void *data)
{
    int result = stbi_write_png(path.c_str(), width, height, 4, data, width * 4);
    if (result == 0)
    {
        LOG_ENGINE_ERROR("Failed to save image to: " + path);
    }
    else
    {
        LOG_ENGINE_INFO("Saved image to: " + path);
    }
}

void *Engine::Texture::LoadImage(const std::string &path, int &width, int &height, int &channels, bool &isHDR,
                                 bool flip)
{
    if (!std::filesystem::exists(path))
        LOG_ENGINE_ASSERT("Texture file does not exist: " + path);

    stbi_set_flip_vertically_on_load(flip);

    isHDR = stbi_is_hdr(path.c_str());
    return isHDR ? (void *)stbi_loadf(path.c_str(), &width, &height, &channels, 0)
                 : (void *)stbi_load(path.c_str(), &width, &height, &channels, 0);
}

void Engine::Texture::FreeImage(void *data) { stbi_image_free(data); }
