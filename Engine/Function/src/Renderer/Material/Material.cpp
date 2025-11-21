#include "Material.hpp"

#include "../Librarys/ShaderLibrary.hpp"

Engine::Material::Material(std::shared_ptr<Shader> shader) : m_Shader(shader)
{
    if (!ShaderLibrary::GetInstance().GetName(shader).empty())
        LOG_ENGINE_INFO("Material created with shader: " + ShaderLibrary::GetInstance().GetName(shader));
    else
    {
        m_Shader = ShaderLibrary::GetInstance().GetDefaultShader();
        LOG_ENGINE_WARN("Material created with unknown shader, default shader assigned");
    }
}

const Engine::MaterialProperty &Engine::Material::GetProperty(const std::string &name) const
{
    if (!Exists(name))
    {
        static MaterialProperty defaultProperty;
        LOG_ENGINE_ERROR("Material::GetProperty: Property not found: " + name);
        return defaultProperty;
    }
    return m_PropertyMap.at(name);
}

void Engine::Material::RemoveProperty(const std::string &name)
{
    if (!Exists(name))
    {
        LOG_ENGINE_WARN("Material::RemoveProperty: Property not found: " + name);
        return;
    }
    m_PropertyMap.erase(name);
    LOG_ENGINE_TRACE("Material::RemoveProperty: Property removed: " + name);
}

void Engine::Material::Bind(const std::string &uniformPrefix,
                            const std::unordered_map<std::string, MaterialProperty> &overrideProperties) const
{
    PROFILE_FUNCTION();

    m_Shader->Bind();
    for (const auto &[name, property] : m_PropertyMap)
    {
        // Check for override
        auto &&overrideIt = overrideProperties.find(name);
        const MaterialProperty &propertyToUse =
            (overrideIt != overrideProperties.end()) ? overrideIt->second : property;

        // Set uniform based on property type
        std::string uniformName = uniformPrefix + "." + name;
        switch (propertyToUse.GetType())
        {
        case MaterialPropertyType::Int:
            m_Shader->SetUniformInt(uniformName, std::get<int>(propertyToUse.GetValue()));
            break;
        case MaterialPropertyType::UInt:
            m_Shader->SetUniformUInt(uniformName, std::get<unsigned int>(propertyToUse.GetValue()));
            break;
        case MaterialPropertyType::Float:
            m_Shader->SetUniformFloat(uniformName, std::get<float>(propertyToUse.GetValue()));
            break;
        case MaterialPropertyType::Vec3:
            m_Shader->SetUniformVec3(uniformName, std::get<glm::vec3>(propertyToUse.GetValue()));
            break;
        case MaterialPropertyType::Vec4:
            m_Shader->SetUniformVec4(uniformName, std::get<glm::vec4>(propertyToUse.GetValue()));
            break;
        // case MaterialPropertyType::Texture: {
        //     int slot = property.GetTexture()->Bind();
        //     m_Shader->SetUniformInt(uniformName, slot);
        // }
        // break;
        default:
            LOG_ENGINE_WARN("Material::Bind: Unsupported MaterialPropertyType for property: " + name);
            break;
        }
    }
}