#include "MaterialLibrary.hpp"

#include "Renderer/Shader/ShaderLibrary.hpp"

MCEngine::MaterialLibrary &MCEngine::MaterialLibrary::GetInstance()
{
    //Instance
    static MaterialLibrary instance;
    return instance;
}

MCEngine::MaterialLibrary::MaterialLibrary()
{
    ENGINE_PROFILE_FUNCTION();
    LOG_ENGINE_INFO("Material Library initialized");
}

std::shared_ptr<MCEngine::Material> MCEngine::MaterialLibrary::Load(const std::string &name)
{
    ENGINE_PROFILE_FUNCTION();

    if (Exists(name))
    {
        LOG_ENGINE_WARN("Material already exists: " + name + ", returning existing material");
        return Get(name);
    }

    // For now, Load just returns nullptr
    // In the future, this can load from file (YAML, JSON, etc.)
    LOG_ENGINE_WARN("Material::Load not implemented yet. Use Create() instead.");
    return nullptr;
}

std::shared_ptr<MCEngine::Material> MCEngine::MaterialLibrary::Create(const std::string &name,
                                                                       std::shared_ptr<Shader> shader)
{
    ENGINE_PROFILE_FUNCTION();

    if (Exists(name))
    {
        LOG_ENGINE_WARN("Material already exists: " + name + ", returning existing material");
        return Get(name);
    }

    if (!shader)
    {
        LOG_ENGINE_ASSERT("MaterialLibrary::Create: Shader is null for material: " + name);
        return nullptr;
    }

    auto material = Material::Create(shader);
    m_Materials[name] = material;
    LOG_ENGINE_TRACE("Material created: " + name);
    return material;
}

void MCEngine::MaterialLibrary::AddMaterial(const std::string &name, const std::shared_ptr<Material> &material)
{
    ENGINE_PROFILE_FUNCTION();

    if (Exists(name))
    {
        LOG_ENGINE_WARN("Material already exists: " + name + ", overwriting");
    }

    if (!material)
    {
        LOG_ENGINE_ASSERT("MaterialLibrary::AddMaterial: Material is null for name: " + name);
        return;
    }

    m_Materials[name] = material;
    LOG_ENGINE_TRACE("Material added: " + name);
}

std::shared_ptr<MCEngine::Material> MCEngine::MaterialLibrary::Get(const std::string &name)
{
    ENGINE_PROFILE_FUNCTION();

    if (!Exists(name))
    {
        LOG_ENGINE_WARN("Material not found: " + name);
        return nullptr;
    }

    return m_Materials[name];
}

bool MCEngine::MaterialLibrary::Exists(const std::string &name) const
{
    return m_Materials.find(name) != m_Materials.end();
}

std::string MCEngine::MaterialLibrary::GetName(const std::shared_ptr<Material> &material) const
{
    for (const auto &[name, ptr] : m_Materials)
    {
        if (ptr == material)
            return name;
    }
    LOG_ENGINE_WARN("Material not found in library");
    return "";
}

