#include "MaterialComponent.hpp"

void MCEngine::MaterialComponent::SetOverrideFloat(const std::string &name, float value)
{
    PropertyOverrides[name] = MaterialProperty(value);
}

void MCEngine::MaterialComponent::SetOverrideVec3(const std::string &name, const glm::vec3 &value)
{
    PropertyOverrides[name] = MaterialProperty(value);
}

void MCEngine::MaterialComponent::SetOverrideTexture2D(const std::string &name,
                                                       const std::shared_ptr<Texture2D> &texture, unsigned int slot)
{
    PropertyOverrides[name] = MaterialProperty(texture, slot);
}

void MCEngine::MaterialComponent::RemoveOverride(const std::string &name)
{
    PropertyOverrides.erase(name);
}

void MCEngine::MaterialComponent::Bind(Shader *shader, const std::string &uniformPrefix) const
{
    ENGINE_PROFILE_FUNCTION();

    if (!MaterialInstance)
    {
        LOG_ENGINE_WARN("MaterialComponent::Bind: Material is null");
        return;
    }

    // First bind the base material
    MaterialInstance->Bind(shader, uniformPrefix);

    // Then apply overrides (these will overwrite base material properties)
    for (const auto &[name, property] : PropertyOverrides)
    {
        std::string uniformName = uniformPrefix + "." + name;

        switch (property.GetType())
        {
        case MaterialPropertyType::Float:
            shader->SetUniformFloat(uniformName, property.GetFloat());
            break;

        case MaterialPropertyType::Int:
            shader->SetUniformInt(uniformName, property.GetInt());
            break;

        case MaterialPropertyType::UInt:
            shader->SetUniformUInt(uniformName, property.GetUInt());
            break;

        case MaterialPropertyType::Vec2: {
            glm::vec2 v = property.GetVec2();
            shader->SetUniformVec3(uniformName, glm::vec3(v.x, v.y, 0.0f));
            break;
        }

        case MaterialPropertyType::Vec3:
            shader->SetUniformVec3(uniformName, property.GetVec3());
            break;

        case MaterialPropertyType::Vec4:
            shader->SetUniformVec4(uniformName, property.GetVec4());
            break;

        case MaterialPropertyType::Mat4:
            shader->SetUniformMat4(uniformName, property.GetMat4());
            break;

        case MaterialPropertyType::Texture2D: {
            auto texture = property.GetTexture2D();
            if (texture)
            {
                unsigned int slot = property.GetTextureSlot();
                texture->Bind(slot);
                shader->SetUniformInt(uniformName, static_cast<int>(slot));
            }
            break;
        }

        default:
            LOG_ENGINE_WARN("MaterialComponent::Bind: Unknown property type for override '" + name + "'");
            break;
        }
    }
}

