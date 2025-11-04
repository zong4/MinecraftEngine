#pragma once

#include "pch.hpp"

struct ImFont;

namespace MCEngine
{

class FontLibrary
{
public:
    static FontLibrary &GetInstance();

    ImFont *GetFont(const std::string &name);
    void AddFont(const std::string &name, const std::string &path, float size);

public:
    void Init(float fontSize, float thinScale);

private:
    std::unordered_map<std::string, ImFont *> m_Fonts;

private:
    FontLibrary() = default;
    ~FontLibrary() = default;

    bool Exists(const std::string &name) const;
};

} // namespace MCEngine