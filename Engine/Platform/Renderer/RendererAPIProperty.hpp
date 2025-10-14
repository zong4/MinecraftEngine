#pragma once

#include "pch.hpp"

namespace MCEngine
{

enum class RendererAPI
{
    None = 0,
    OpenGL = 1
};

class RendererAPIProperty
{
public:
    RendererAPIProperty(RendererAPI api, int majorVersion, int minorVersion);

    RendererAPI GetAPI() const { return m_API; }
    int GetMajorVersion() const { return m_MajorVersion; }
    int GetMinorVersion() const { return m_MinorVersion; }

private:
    RendererAPI m_API;
    int m_MajorVersion;
    int m_MinorVersion;
};

} // namespace MCEngine