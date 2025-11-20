#pragma once

#include "Texture.hpp"

namespace Engine
{

class TextureCube : public Texture
{
public:
    TextureCube(const glm::vec4 &color);
    TextureCube(const std::array<std::string, 6> &faces);
    virtual ~TextureCube() override;

    static std::shared_ptr<TextureCube> WhiteTexture();

public:
    virtual void Bind(unsigned int slot) const override;
    virtual void Unbind() const override;
};

} // namespace Engine