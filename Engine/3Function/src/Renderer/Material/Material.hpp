#pragma once

#include "MaterialProperty.hpp"

namespace Engine
{

class Material
{
public:
    Material(const std::shared_ptr<Shader> &shader);

    // Operators
    operator bool() const { return m_Shader != nullptr; }

    // Getters
    std::shared_ptr<Shader> GetShader() const { return m_Shader; }
    const MaterialProperty &GetProperty(const std::string &name) const;
    const std::unordered_map<std::string, MaterialProperty> &GetPropertyMap() const { return m_PropertyMap; }

    // Setters
    void SetShader(const std::shared_ptr<Shader> &shader) { m_Shader = shader; }
    void AddProperty(const std::string &name, const MaterialProperty &property) { m_PropertyMap[name] = property; }
    void RemoveProperty(const std::string &name);

public:
    void Bind(const std::string &uniformPrefix,
              const std::unordered_map<std::string, MaterialProperty> &overridePropertyMap = {}) const;

private:
    std::shared_ptr<Shader> m_Shader;
    std::unordered_map<std::string, MaterialProperty> m_PropertyMap;

private:
    bool Exists(const std::string &name) const { return m_PropertyMap.find(name) != m_PropertyMap.end(); }
};

} // namespace Engine
