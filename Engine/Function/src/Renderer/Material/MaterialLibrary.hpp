#pragma once

#include "Material.hpp"

namespace Engine
{

class MaterialLibrary
{
public:
    static MaterialLibrary &GetInstance();

    // Load / Create Material
    std::shared_ptr<Material> Load(const std::string &name);
    std::shared_ptr<Material> Create(const std::string &name, std::shared_ptr<Shader> shader);
    void AddMaterial(const std::string &name, const std::shared_ptr<Material> &material);

    // Get Material
    std::shared_ptr<Material> Get(const std::string &name);
    bool Exists(const std::string &name) const;

    // Get material name
    std::string GetName(const std::shared_ptr<Material> &material) const;

private:
    std::unordered_map<std::string, std::shared_ptr<Material>> m_Materials;

private:
    MaterialLibrary();
    ~MaterialLibrary() = default;
};

} // namespace Engine