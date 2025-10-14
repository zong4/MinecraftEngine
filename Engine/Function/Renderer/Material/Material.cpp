#include "Material.hpp"

MCEngine::Material::Material(const glm::vec4 &color, float ambient, float diffuse, float specular, float shininess)
    : m_Color(color), m_AmbientStrength(ambient), m_DiffuseStrength(diffuse), m_SpecularStrength(specular),
      m_Shininess(shininess)
{
    LOG_ENGINE_TRACE("Material created with " + ToString());
}

void MCEngine::Material::Bind(const std::shared_ptr<MCEngine::Shader> &shader, const std::string &name) const
{
    ENGINE_PROFILE_FUNCTION();

    shader->SetUniformVec4(name + ".Color", m_Color);
    shader->SetUniformFloat(name + ".AmbientStrength", m_AmbientStrength);
    shader->SetUniformFloat(name + ".DiffuseStrength", m_DiffuseStrength);
    shader->SetUniformFloat(name + ".SpecularStrength", m_SpecularStrength);
    shader->SetUniformFloat(name + ".Shininess", m_Shininess);
}

std::string MCEngine::Material::ToString() const
{
    return "Material(Color: " + MCEngine::Math::ToString(m_Color) + ", Ambient: " + std::to_string(m_AmbientStrength) +
           ", Diffuse: " + std::to_string(m_DiffuseStrength) + ", Specular: " + std::to_string(m_SpecularStrength) +
           ", Shininess: " + std::to_string(m_Shininess) + ")";
}