#pragma once

#include "Texture.hpp"
#include <array>

namespace Engine
{

class TextureCube : public Texture
{
public:
    virtual ~TextureCube() override = default;
    static std::shared_ptr<TextureCube> Create(const std::array<std::string, 6> &faces);
    static std::shared_ptr<TextureCube> WhiteTexture();

protected:
    TextureCube() = default;
};

} // namespace Engine