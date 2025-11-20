#include "RendererAPIProperty.hpp"

MCEngine::RendererAPIProperty::RendererAPIProperty(RendererAPI api, int majorVersion, int minorVersion)
    : m_API(api), m_MajorVersion(majorVersion), m_MinorVersion(minorVersion)
{
    LOG_ENGINE_INFO("RendererAPIProperty created with API: " + std::to_string(static_cast<int>(api)) +
                    ", Version: " + std::to_string(majorVersion) + "." + std::to_string(minorVersion));
}