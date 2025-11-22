#include "Texture.hpp"

#include <filesystem>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

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
