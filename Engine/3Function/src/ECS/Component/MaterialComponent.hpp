#pragma once

#include "../../AssetsManager/MaterialsManager.hpp"

namespace Engine
{

struct MaterialComponent
{
    std::shared_ptr<Material> MaterialInstance;

public:
    MaterialComponent(
        const std::shared_ptr<Material> &material = MaterialsManager::GetInstance().GetMaterial("Default"))
        : MaterialInstance(material), m_PropertysMap(material->GetPropertyMap())
    {
    }

    // Getters
    MaterialProperty &GetProperty(const std::string &name)
    {
        // Find property in override map
        auto &&it = m_PropertysMap.find(name);
        if (it != m_PropertysMap.end())
            return it->second;

        // Property not found
        static MaterialProperty emptyProperty;
        LOG_ENGINE_WARN("MaterialComponent: Property '" + name + "' not found in MaterialInstance");
        return emptyProperty;
    }

    // Setters
    void SetProperty(const std::string &name, const MaterialProperty &property) { m_PropertysMap[name] = property; }
    void ClearProperty(const std::string &name) { m_PropertysMap[name] = MaterialInstance->GetProperty(name); }

public:
    void Bind(const std::string &uniformPrefix = "u_Material") const
    {
        MaterialInstance->Bind(uniformPrefix, m_PropertysMap);
    }

private:
    std::unordered_map<std::string, MaterialProperty> m_PropertysMap;
};

} // namespace Engine