#pragma once

#include <Core.hpp>

namespace Engine
{

enum class RendererAPI
{
    None = 0,
    OpenGL = 1,
    Vulkan = 2
};

class RendererProperty
{
public:
    static RendererProperty GetInstance()
    {
#ifdef __APPLE__
        static RendererProperty instance = RendererProperty(RendererAPI::OpenGL, 4, 1);
#else
        static RendererProperty instance = RendererProperty(RendererAPI::OpenGL, 4, 2);
#endif
        return instance;
    }

    RendererAPI GetAPI() const { return m_API; }
    int GetMajorVersion() const { return m_MajorVersion; }
    int GetMinorVersion() const { return m_MinorVersion; }

private:
    RendererAPI m_API;
    int m_MajorVersion;
    int m_MinorVersion;

private:
    RendererProperty(RendererAPI api, int majorVersion, int minorVersion)
        : m_API(api), m_MajorVersion(majorVersion), m_MinorVersion(minorVersion)
    {
    }
};

} // namespace Engine