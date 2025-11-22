#pragma once

#include <Core.hpp>

namespace Engine
{

class Texture
{
public:
    Texture() = default;
    virtual ~Texture() = default;

    // Getters
    virtual unsigned int GetRendererID() const { return m_RendererID; }

public:
    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;
    virtual void Active(unsigned int slot) const = 0;

protected:
    unsigned int m_RendererID = 0;

protected:
    void *LoadImage(const std::string &path, int &width, int &height, int &channels, bool &isHDR, bool flipVertically);
    void FreeImage(void *data);
};

} // namespace Engine