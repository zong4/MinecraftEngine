#include "FontLibrary.hpp"

#include <imgui.h>

MCEngine::FontLibrary &MCEngine::FontLibrary::GetInstance()
{
    static FontLibrary instance;
    return instance;
}

ImFont *MCEngine::FontLibrary::GetFont(const std::string &name)
{
    ENGINE_PROFILE_FUNCTION();

    if (!Exists(name))
    {
        LOG_ENGINE_ERROR("Font not found: " + name);
        return nullptr;
    }
    return m_Fonts[name];
}

void MCEngine::FontLibrary::AddFont(const std::string &name, const std::string &path, float size)
{
    ENGINE_PROFILE_FUNCTION();

    if (Exists(name))
    {
        LOG_ENGINE_WARN("Font already exists: " + name);
        return;
    }

    ImFont *font = ImGui::GetIO().Fonts->AddFontFromFileTTF(path.c_str(), size);
    if (!font)
    {
        LOG_ENGINE_ERROR("Failed to load font from path: " + path);
        return;
    }

    m_Fonts[name] = font;
    LOG_ENGINE_TRACE("Font added: " + name + " from path: " + path + " with size: " + std::to_string(size));
}

void MCEngine::FontLibrary::Init(float fontSize, float thinScale)
{
    ENGINE_PROFILE_FUNCTION();

    std::filesystem::path path(std::string(PROJECT_ROOT) + "/Engine/Resources/Fonts/");
    if (!std::filesystem::exists(path))
    {
        LOG_ENGINE_ERROR("Font directory does not exist: " + path.string());
        return;
    }

    for (const auto &entry : std::filesystem::recursive_directory_iterator(path))
    {
        if (entry.is_regular_file() && entry.path().extension() == ".ttf")
        {
            AddFont(entry.path().stem().string(), entry.path().string(), fontSize);
            AddFont(entry.path().stem().string() + "-Thin", entry.path().string(), fontSize * thinScale);
        }
    }

    LOG_ENGINE_INFO("FontLibrary initialized with font size: " + std::to_string(fontSize) +
                    " and thin scale: " + std::to_string(thinScale));
}

bool MCEngine::FontLibrary::Exists(const std::string &name) const { return m_Fonts.find(name) != m_Fonts.end(); }