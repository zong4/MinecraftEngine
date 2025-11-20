#include "Material.hpp"

#include <glad/glad.h>

Engine::Material::Material(std::shared_ptr<Shader> shader) : m_Shader(shader)
{
    ENGINE_PROFILE_FUNCTION();
    LOG_ENGINE_TRACE("Material created with shader");
}

void Engine::Material::SetFloat(const std::string &name, float value) { m_Properties[name] = MaterialProperty(value); }

void Engine::Material::SetInt(const std::string &name, int value) { m_Properties[name] = MaterialProperty(value); }

void Engine::Material::SetUInt(const std::string &name, unsigned int value)
{
    m_Properties[name] = MaterialProperty(value);
}

void Engine::Material::SetVec2(const std::string &name, const glm::vec2 &value)
{
    m_Properties[name] = MaterialProperty(value);
}

void Engine::Material::SetVec3(const std::string &name, const glm::vec3 &value)
{
    m_Properties[name] = MaterialProperty(value);
}

void Engine::Material::SetVec4(const std::string &name, const glm::vec4 &value)
{
    m_Properties[name] = MaterialProperty(value);
}

void Engine::Material::SetMat4(const std::string &name, const glm::mat4 &value)
{
    m_Properties[name] = MaterialProperty(value);
}

void Engine::Material::SetTexture2D(const std::string &name, const std::shared_ptr<Texture2D> &texture,
                                    unsigned int slot)
{
    m_Properties[name] = MaterialProperty(texture, slot);
}

Engine::MaterialProperty Engine::Material::GetProperty(const std::string &name) const
{
    auto it = m_Properties.find(name);
    if (it != m_Properties.end())
    {
        return it->second;
    }
    LOG_ENGINE_WARN("Material: Property '" + name + "' not found");
    return MaterialProperty();
}

bool Engine::Material::HasProperty(const std::string &name) const
{
    return m_Properties.find(name) != m_Properties.end();
}

void Engine::Material::RemoveProperty(const std::string &name)
{
    m_Properties.erase(name);
    // Also remove from cache
    m_UniformLocationCache.erase(name);
}

void Engine::Material::Bind(Shader *shader, const std::string &uniformPrefix) const
{
    ENGINE_PROFILE_FUNCTION();

    // Use provided shader or default to material's shader
    Shader *targetShader = shader ? shader : m_Shader.get();
    if (!targetShader)
    {
        LOG_ENGINE_WARN("Material::Bind: No shader available");
        return;
    }

    // Bind all properties to shader
    for (const auto &[name, property] : m_Properties)
    {
        std::string uniformName = uniformPrefix + "." + name;

        switch (property.GetType())
        {
        case MaterialPropertyType::Float:
            targetShader->SetUniformFloat(uniformName, property.GetFloat());
            break;

        case MaterialPropertyType::Int:
            targetShader->SetUniformInt(uniformName, property.GetInt());
            break;

        case MaterialPropertyType::UInt:
            targetShader->SetUniformUInt(uniformName, property.GetUInt());
            break;

        case MaterialPropertyType::Vec2: {
            glm::vec2 v = property.GetVec2();
            // Note: Shader class might need SetUniformVec2, for now we'll use vec3 with z=0
            targetShader->SetUniformVec3(uniformName, glm::vec3(v.x, v.y, 0.0f));
            break;
        }

        case MaterialPropertyType::Vec3:
            targetShader->SetUniformVec3(uniformName, property.GetVec3());
            break;

        case MaterialPropertyType::Vec4:
            targetShader->SetUniformVec4(uniformName, property.GetVec4());
            break;

        case MaterialPropertyType::Mat4:
            targetShader->SetUniformMat4(uniformName, property.GetMat4());
            break;

        case MaterialPropertyType::Texture2D: {
            auto texture = property.GetTexture2D();
            if (texture)
            {
                unsigned int slot = property.GetTextureSlot();
                texture->Bind(slot);
                targetShader->SetUniformInt(uniformName, static_cast<int>(slot));
            }
            break;
        }

        default:
            LOG_ENGINE_WARN("Material::Bind: Unknown property type for '" + name + "'");
            break;
        }
    }
}

std::string Engine::Material::ToString() const
{
    std::string result = std::string("Material(Shader: ") + (m_Shader ? "Valid" : "Null") + ", Properties: {";
    bool first = true;
    for (const auto &[name, property] : m_Properties)
    {
        if (!first)
            result += ", ";
        result += name + "=" + property.ToString();
        first = false;
    }
    result += "})";
    return result;
}

std::shared_ptr<Engine::Material> Engine::Material::Create(std::shared_ptr<Shader> shader)
{
    return std::make_shared<Material>(shader);
}

std::shared_ptr<Engine::Material> Engine::Material::CreateDefault(std::shared_ptr<Shader> shader)
{
    auto material = std::make_shared<Material>(shader);
    // Set default PBR values
    material->SetVec3("Albedo", glm::vec3(1.0f));
    material->SetFloat("Metallic", 0.0f);
    material->SetFloat("Roughness", 0.5f);
    material->SetFloat("AO", 1.0f);
    return material;
}
