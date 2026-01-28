#pragma once

#include "../Renderer/Material/Material.hpp"

namespace Engine
{

class MaterialsManager
{
public:
    static MaterialsManager &GetInstance();

    // Getters
    std::string GetName(const std::shared_ptr<Material> &material) const;
    std::shared_ptr<Material> GetMaterial(const std::string &name);

    // Setters
    void AddMaterial(const std::string &name, const std::shared_ptr<Material> &material);

private:
    std::unordered_map<std::string, std::shared_ptr<Material>> m_MaterialsMap;

private:
    MaterialsManager();
    ~MaterialsManager() = default;

    bool Exists(const std::string &name) const { return m_MaterialsMap.find(name) != m_MaterialsMap.end(); };
};

} // namespace Engine