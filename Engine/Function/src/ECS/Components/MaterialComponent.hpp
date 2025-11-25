#pragma once

#include "../../Renderers/Material/MaterialLibrary.hpp"

namespace Engine
{

struct MaterialComponent
{
    std::shared_ptr<Material> MaterialInstance;

public:
    MaterialComponent(
        const std::shared_ptr<Material> &material = MaterialLibrary::GetInstance().GetMaterial("DefaultMaterial"))
        : MaterialInstance(material), PropertyMap(material->GetPropertyMap())
    {
    }

    MaterialProperty &GetProperty(const std::string &name)
    {
        static MaterialProperty nullProperty;

        auto &&it = PropertyMap.find(name);
        if (it != PropertyMap.end())
            return it->second;
        LOG_ENGINE_WARN("MaterialComponent: Property '" + name + "' not found in MaterialInstance");
        return nullProperty;
    }
    void SetProperty(const std::string &name, const MaterialProperty &property) { PropertyMap[name] = property; }
    void ClearProperty(const std::string &name) { PropertyMap[name] = MaterialInstance->GetProperty(name); }

public:
    void Bind(const std::string &uniformPrefix = "u_Material") const
    {
        MaterialInstance->Bind(uniformPrefix, PropertyMap);
    }

private:
    std::unordered_map<std::string, MaterialProperty> PropertyMap;
};

} // namespace Engine