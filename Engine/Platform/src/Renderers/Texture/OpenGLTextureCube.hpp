#pragma once

#include "TextureCube.hpp"

namespace Engine
{

class OpenGLTextureCube : public TextureCube
{
public:
    OpenGLTextureCube(const glm::vec4 &color);
    OpenGLTextureCube(const std::array<std::string, 6> &faces);
    virtual ~OpenGLTextureCube() override;

public:
    void Bind(unsigned int slot) const override;
    void Unbind() const override;
};

} // namespace Engine