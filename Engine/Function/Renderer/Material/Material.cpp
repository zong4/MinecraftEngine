#include "Material.hpp"

MCEngine::Material::Material(const glm::vec4 &color, float ambient, float diffuse, float specular, float shininess)
    : Color(color), AmbientStrength(ambient), DiffuseStrength(diffuse), SpecularStrength(specular), Shininess(shininess)
{
    LOG_ENGINE_TRACE("Material created with " + ToString());
}

void MCEngine::Material::Bind(const std::shared_ptr<MCEngine::Shader> &shader, const std::string &name) const
{
    ENGINE_PROFILE_FUNCTION();

    shader->SetUniformVec4(name + ".Color", Color);
    shader->SetUniformFloat(name + ".AmbientStrength", AmbientStrength);
    shader->SetUniformFloat(name + ".DiffuseStrength", DiffuseStrength);
    shader->SetUniformFloat(name + ".SpecularStrength", SpecularStrength);
    shader->SetUniformFloat(name + ".Shininess", Shininess);
}

std::string MCEngine::Material::ToString() const
{
    return "Material(Color: " + MCEngine::Math::ToString(Color) + ", Ambient: " + std::to_string(AmbientStrength) +
           ", Diffuse: " + std::to_string(DiffuseStrength) + ", Specular: " + std::to_string(SpecularStrength) +
           ", Shininess: " + std::to_string(Shininess) + ")";
}