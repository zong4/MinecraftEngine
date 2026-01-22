#include "MaterialsManager.hpp"

#include "ShadersManager.hpp"

Engine::MaterialsManager &Engine::MaterialsManager::GetInstance()
{
    static MaterialsManager instance;
    return instance;
}

std::string Engine::MaterialsManager::GetName(const std::shared_ptr<Material> &material) const
{
    for (const auto &[name, ptr] : m_MaterialsMap)
    {
        if (ptr == material)
            return name;
    }
    LOG_ENGINE_ERROR("Material not found in library");
    return "";
}

std::shared_ptr<Engine::Material> Engine::MaterialsManager::GetMaterial(const std::string &name)
{
    if (!Exists(name))
    {
        LOG_ENGINE_ERROR("Material not found: " + name);
        return nullptr;
    }
    return m_MaterialsMap[name];
}

void Engine::MaterialsManager::AddMaterial(const std::string &name, const std::shared_ptr<Material> &material)
{
    // Validate material
    if (!material)
    {
        LOG_ENGINE_ERROR("Cannot add null material: " + name);
        return;
    }

    // Check if material already exists
    if (Exists(name))
    {
        LOG_ENGINE_WARN("Material already exists: " + name + ", overwriting");
    }

    // Add material to map
    m_MaterialsMap[name] = material;
    LOG_ENGINE_TRACE("Material added: " + name);
}

Engine::MaterialsManager::MaterialsManager()
{
    PROFILE_FUNCTION();

    // Load all materials
    auto &&shader = Engine::ShadersManager::GetInstance().GetShader("BlinnPhong");
    auto &&whiteMaterial = std::make_shared<Engine::Material>(shader);
    whiteMaterial->AddProperty("Color", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    whiteMaterial->AddProperty("AmbientStrength", 0.1f);
    whiteMaterial->AddProperty("DiffuseStrength", 0.8f);
    whiteMaterial->AddProperty("SpecularStrength", 0.5f);
    whiteMaterial->AddProperty("Shininess", 32.0f);
    AddMaterial("Default", whiteMaterial);

    LOG_ENGINE_INFO("MaterialsManager initialized");
}