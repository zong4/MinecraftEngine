#include "MaterialLibrary.hpp"

#include "../Librarys/ShaderLibrary.hpp"

Engine::MaterialLibrary &Engine::MaterialLibrary::GetInstance()
{
    static MaterialLibrary instance;
    return instance;
}

std::string Engine::MaterialLibrary::GetName(const std::shared_ptr<Material> &material) const
{
    for (const auto &[name, ptr] : m_MaterialMap)
    {
        if (ptr == material)
            return name;
    }
    LOG_ENGINE_ERROR("Material not found in library");
    return "";
}

void Engine::MaterialLibrary::AddMaterial(const std::string &name, const std::shared_ptr<Material> &material)
{
    if (!material)
    {
        LOG_ENGINE_ASSERT("Cannot add null material: " + name);
        return;
    }

    if (Exists(name))
    {
        LOG_ENGINE_WARN("Material already exists: " + name);
        return;
    }
    m_MaterialMap[name] = material;
    LOG_ENGINE_TRACE("Material added: " + name);
}

std::shared_ptr<Engine::Material> Engine::MaterialLibrary::GetMaterial(const std::string &name)
{
    if (!Exists(name))
    {
        LOG_ENGINE_ERROR("Material not found: " + name);
        return nullptr;
    }
    return m_MaterialMap[name];
}

Engine::MaterialLibrary::MaterialLibrary()
{
    PROFILE_FUNCTION();

    // Create a material using the new system
    auto &&shader = Engine::ShaderLibrary::GetInstance().GetShader("BlinnPhong");
    auto &&redMaterial = std::make_shared<Engine::Material>(shader);

    redMaterial->AddProperty("Color", glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
    redMaterial->AddProperty("AmbientStrength", 0.1f);
    redMaterial->AddProperty("DiffuseStrength", 0.8f);
    redMaterial->AddProperty("SpecularStrength", 0.5f);
    redMaterial->AddProperty("Shininess", 32.0f);

    AddMaterial("RedMaterial", redMaterial);
}