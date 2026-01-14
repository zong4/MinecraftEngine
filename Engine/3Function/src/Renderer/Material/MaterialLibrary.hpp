#pragma once

#include "Material.hpp"

namespace Engine
{

class MaterialLibrary
{
public:
    static MaterialLibrary &GetInstance();

    std::string GetName(const std::shared_ptr<Material> &material) const;
    std::shared_ptr<Material> GetMaterial(const std::string &name);
    void AddMaterial(const std::string &name, const std::shared_ptr<Material> &material);

private:
    std::unordered_map<std::string, std::shared_ptr<Material>> m_MaterialMap;

private:
    MaterialLibrary();
    ~MaterialLibrary() = default;

    bool Exists(const std::string &name) const { return m_MaterialMap.find(name) != m_MaterialMap.end(); };
};

} // namespace Engine