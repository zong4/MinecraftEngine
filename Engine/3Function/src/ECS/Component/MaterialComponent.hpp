#pragma once

#include "../../Renderer/Material/Material.hpp"

namespace Engine
{

struct MaterialComponent
{
public:
    MaterialComponent(const std::shared_ptr<Material> &material)
        : m_Material(material), m_PropertysMap(material->GetPropertyMap())
    {
    }

    // Getters
    std::shared_ptr<Material> GetMaterial() const { return m_Material; }
    MaterialProperty &GetProperty(const std::string &name)
    {
        // Find property in override map
        auto &&it = m_PropertysMap.find(name);
        if (it != m_PropertysMap.end())
            return it->second;

        // Property not found
        static MaterialProperty emptyProperty;
        LOG_ENGINE_WARN("MaterialComponent: Property '" + name + "' not found in Material");
        return emptyProperty;
    }
    const MaterialProperty &GetProperty(const std::string &name) const
    {
        // Find property in override map
        auto &&it = m_PropertysMap.find(name);
        if (it != m_PropertysMap.end())
            return it->second;

        // Property not found
        static MaterialProperty emptyProperty;
        LOG_ENGINE_WARN("MaterialComponent: Property '" + name + "' not found in Material");
        return emptyProperty;
    }

    // Setters
    void SetMaterial(const std::shared_ptr<Material> &material)
    {
        m_Material = material;
        m_PropertysMap = material->GetPropertyMap();
    }
    void SetProperty(const std::string &name, const MaterialProperty &property) { m_PropertysMap[name] = property; }
    void ClearProperty(const std::string &name) { m_PropertysMap[name] = m_Material->GetProperty(name); }

public:
    void Bind(const std::string &uniformPrefix = "u_Material") const
    {
        m_Material->Bind(uniformPrefix, m_PropertysMap);
    }

private:
    std::shared_ptr<Material> m_Material;
    std::unordered_map<std::string, MaterialProperty> m_PropertysMap;
};

} // namespace Engine