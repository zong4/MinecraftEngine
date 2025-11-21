#include "MaterialLibrary.hpp"

#include "../Librarys/ShaderLibrary.hpp"

Engine::MaterialLibrary &Engine::MaterialLibrary::GetInstance()
{
    // Instance
    static MaterialLibrary instance;
    return instance;
}

Engine::MaterialLibrary::MaterialLibrary()
{
    PROFILE_FUNCTION();
    LOG_ENGINE_INFO("Material Library initialized");
}

std::shared_ptr<Engine::Material> Engine::MaterialLibrary::Load(const std::string &name)
{
    PROFILE_FUNCTION();

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

std::shared_ptr<Engine::Material> Engine::MaterialLibrary::Create(const std::string &name,
                                                                  std::shared_ptr<Shader> shader)
{
    PROFILE_FUNCTION();

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

void Engine::MaterialLibrary::AddMaterial(const std::string &name, const std::shared_ptr<Material> &material)
{
    PROFILE_FUNCTION();

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

std::shared_ptr<Engine::Material> Engine::MaterialLibrary::Get(const std::string &name)
{
    PROFILE_FUNCTION();

    if (!Exists(name))
    {
        LOG_ENGINE_WARN("Material not found: " + name);
        return nullptr;
    }

    return m_Materials[name];
}

bool Engine::MaterialLibrary::Exists(const std::string &name) const
{
    return m_Materials.find(name) != m_Materials.end();
}

std::string Engine::MaterialLibrary::GetName(const std::shared_ptr<Material> &material) const
{
    for (const auto &[name, ptr] : m_Materials)
    {
        if (ptr == material)
            return name;
    }
    LOG_ENGINE_WARN("Material not found in library");
    return "";
}
