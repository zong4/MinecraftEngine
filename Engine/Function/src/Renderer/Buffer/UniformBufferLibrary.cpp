#include "UniformBufferLibrary.hpp"

Engine::UniformBufferLibrary &Engine::UniformBufferLibrary::GetInstance()
{
    static UniformBufferLibrary instance;
    return instance;
}

std::shared_ptr<Engine::UniformBuffer> Engine::UniformBufferLibrary::GetUniformBuffer(const std::string &name)
{
    PROFILE_FUNCTION();

    if (!Exists(name))
    {
        LOG_ENGINE_ASSERT("UniformBuffer not found: " + name);
        return nullptr;
    }
    return m_UniformBufferMap[name];
}

void Engine::UniformBufferLibrary::AddUniformBuffer(const std::string &name, size_t size, unsigned int binding)
{
    PROFILE_FUNCTION();

    if (Exists(name))
    {
        LOG_ENGINE_WARN("UniformBuffer already exists: " + name);
        return;
    }
    m_UniformBufferMap[name] = std::make_shared<UniformBuffer>(size, binding);
    LOG_ENGINE_INFO("UniformBuffer added: " + name);
}

void Engine::UniformBufferLibrary::RemoveUniformBuffer(const std::string &name)
{
    PROFILE_FUNCTION();

    if (!Exists(name))
    {
        LOG_ENGINE_WARN("UniformBuffer not found: " + name);
        return;
    }
    m_UniformBufferMap.erase(name);
    LOG_ENGINE_INFO("UniformBuffer removed: " + name);
}

void Engine::UniformBufferLibrary::UpdateUniformBuffer(const std::string &name,
                                                       const std::initializer_list<UniformBufferData> &dataList)
{
    PROFILE_FUNCTION();

    if (!Exists(name))
    {
        LOG_ENGINE_WARN("UniformBuffer not found: " + name);
        return;
    }
    m_UniformBufferMap[name]->SetData(dataList);
}

Engine::UniformBufferLibrary::UniformBufferLibrary()
{
    PROFILE_FUNCTION();

    // todo: check size
    AddUniformBuffer("UniformBuffer0", sizeof(glm::mat4) * 10, 0);
}

bool Engine::UniformBufferLibrary::Exists(const std::string &name) const
{
    return m_UniformBufferMap.find(name) != m_UniformBufferMap.end();
}
