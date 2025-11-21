#pragma once

#include "../../Renderer/Material/MaterialLibrary.hpp"

namespace Engine
{

struct MaterialComponent
{
    std::shared_ptr<Material> MaterialInstance;

public:
    MaterialComponent(
        const std::shared_ptr<Material> &material = MaterialLibrary::GetInstance().GetMaterial("RedMaterial"))
        : MaterialInstance(material), PropertyOverrideMap(material->GetProperties())
    {
    }

    // Setters
    void SetOverride(const std::string &name, const MaterialProperty &property)
    {
        PropertyOverrideMap[name] = property;
    }
    void ClearOverride(const std::string &name) { PropertyOverrideMap[name] = MaterialInstance->GetProperty(name); }

public:
    void Bind(const std::string &uniformPrefix = "u_Material") const
    {
        MaterialInstance->Bind(uniformPrefix, PropertyOverrideMap);
    }

private:
    std::unordered_map<std::string, MaterialProperty> PropertyOverrideMap;
};

} // namespace Engine