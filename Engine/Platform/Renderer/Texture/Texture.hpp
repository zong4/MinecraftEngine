#pragma once

#include "Renderer/RendererCommand.hpp"

namespace MCEngine
{

class Texture
{
public:
    Texture() = default;
    virtual ~Texture() = default;

    // Getters
    virtual unsigned int GetRendererID() const { return m_RendererID; }

public:
    virtual void Bind(unsigned int slot) const = 0;
    virtual void Unbind() const = 0;

protected:
    unsigned int m_RendererID = 0;

protected:
    unsigned char *LoadImage(const std::string &path, int &width, int &height, unsigned int &internalFormat,
                             unsigned int &format, bool flipVertically);
    void FreeImage(unsigned char *data);
};

} // namespace MCEngine