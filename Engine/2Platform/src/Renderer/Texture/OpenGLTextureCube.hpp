#pragma once

#include "TextureCube.hpp"

namespace Engine
{

class OpenGLTextureCube : public TextureCube
{
public:
    OpenGLTextureCube(void *data);
    OpenGLTextureCube(const std::array<std::string, 6> &faces);
    virtual ~OpenGLTextureCube() override;

public:
    void Active(unsigned int slot) const override;

protected:
    void Bind() const override;
    void Unbind() const override;
};

} // namespace Engine