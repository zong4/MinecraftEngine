#pragma once

#include "pch.hpp"
#include "Renderer/Material/Material.hpp"
#include "Renderer/Material/MaterialProperty.hpp"
#include "Renderer/Material/MaterialLibrary.hpp"

namespace MCEngine
{

// New MaterialComponent - uses the new Material system (Unity style)
struct MaterialComponent
{
    std::shared_ptr<Material> MaterialInstance;

    // Optional: Runtime property overrides (similar to MaterialPropertyBlock in Unity)
    // These override the base material properties for this specific instance
    std::unordered_map<std::string, MaterialProperty> PropertyOverrides;

public:
    MaterialComponent(const std::shared_ptr<Material> &material = nullptr) : MaterialInstance(material)
    {
    }

    // Convenience constructor - create from material name
    MaterialComponent(const std::string &materialName)
        : MaterialInstance(MaterialLibrary::GetInstance().Get(materialName))
    {
    }

    // Get material (with fallback)
    std::shared_ptr<Material> GetMaterial() const { return MaterialInstance; }

    // Set material
    void SetMaterial(const std::shared_ptr<Material> &material) { MaterialInstance = material; }

    // Set material by name
    void SetMaterial(const std::string &materialName)
    {
        MaterialInstance = MaterialLibrary::GetInstance().Get(materialName);
    }

    // Property override methods (for runtime modifications)
    void SetOverrideFloat(const std::string &name, float value);
    void SetOverrideVec3(const std::string &name, const glm::vec3 &value);
    void SetOverrideTexture2D(const std::string &name, const std::shared_ptr<Texture2D> &texture,
                               unsigned int slot = 0);
    void RemoveOverride(const std::string &name);
    void ClearOverrides() { PropertyOverrides.clear(); }

    // Bind material to shader (applies base material + overrides)
    void Bind(Shader *shader, const std::string &uniformPrefix = "u_Material") const;
};

} // namespace MCEngine