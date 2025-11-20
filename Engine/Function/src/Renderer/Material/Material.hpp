#pragma once

#include "MaterialProperty.hpp"
#include "pch.hpp"

namespace Engine
{

// Material class - Unity style with dynamic properties
class Material
{
public:
    Material(std::shared_ptr<Shader> shader);
    Material(const Material &other) = default;
    Material &operator=(const Material &other) = default;
    ~Material() = default;

    // Property setters
    void SetFloat(const std::string &name, float value);
    void SetInt(const std::string &name, int value);
    void SetUInt(const std::string &name, unsigned int value);
    void SetVec2(const std::string &name, const glm::vec2 &value);
    void SetVec3(const std::string &name, const glm::vec3 &value);
    void SetVec4(const std::string &name, const glm::vec4 &value);
    void SetMat4(const std::string &name, const glm::mat4 &value);
    void SetTexture2D(const std::string &name, const std::shared_ptr<Texture2D> &texture, unsigned int slot = 0);

    // Property getters
    MaterialProperty GetProperty(const std::string &name) const;
    bool HasProperty(const std::string &name) const;

    // Remove property
    void RemoveProperty(const std::string &name);

    // Get all properties (for serialization)
    const std::unordered_map<std::string, MaterialProperty> &GetProperties() const { return m_Properties; }

    // Shader
    std::shared_ptr<Shader> GetShader() const { return m_Shader; }
    void SetShader(std::shared_ptr<Shader> shader) { m_Shader = shader; }

    // Bind material to shader
    void Bind(Shader *shader = nullptr, const std::string &uniformPrefix = "u_Material") const;

    // Utility
    std::string ToString() const;

    // Static factory methods
    static std::shared_ptr<Material> Create(std::shared_ptr<Shader> shader);
    static std::shared_ptr<Material> CreateDefault(std::shared_ptr<Shader> shader);

private:
    std::shared_ptr<Shader> m_Shader;
    std::unordered_map<std::string, MaterialProperty> m_Properties;

    // Uniform location cache for performance
    mutable std::unordered_map<std::string, int> m_UniformLocationCache;
};

} // namespace Engine
