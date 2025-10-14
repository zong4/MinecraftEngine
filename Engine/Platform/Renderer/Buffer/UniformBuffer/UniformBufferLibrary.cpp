#include "UniformBufferLibrary.hpp"

MCEngine::UniformBufferLibrary &MCEngine::UniformBufferLibrary::GetInstance()
{
    static UniformBufferLibrary instance;
    return instance;
}

std::shared_ptr<MCEngine::UniformBuffer> MCEngine::UniformBufferLibrary::GetUniformBuffer(const std::string &name)
{
    ENGINE_PROFILE_FUNCTION();

    if (!Exists(name))
    {
        LOG_ENGINE_ERROR("UniformBuffer not found: " + name);
        return nullptr;
    }
    return m_UniformBufferMap[name];
}

void MCEngine::UniformBufferLibrary::AddUniformBuffer(const std::string &name, size_t size, unsigned int binding)
{
    ENGINE_PROFILE_FUNCTION();

    if (Exists(name))
    {
        LOG_ENGINE_WARN("UniformBuffer already exists: " + name);
        return;
    }
    m_UniformBufferMap[name] = std::make_shared<UniformBuffer>(size, binding);
    LOG_ENGINE_INFO("UniformBuffer added: " + name);
}

void MCEngine::UniformBufferLibrary::RemoveUniformBuffer(const std::string &name)
{
    ENGINE_PROFILE_FUNCTION();

    if (!Exists(name))
    {
        LOG_ENGINE_WARN("UniformBuffer not found: " + name);
        return;
    }
    m_UniformBufferMap.erase(name);
    LOG_ENGINE_INFO("UniformBuffer removed: " + name);
}

void MCEngine::UniformBufferLibrary::UpdateUniformBuffer(const std::string &name,
                                                         const std::initializer_list<UniformBufferData> &dataList)
{
    ENGINE_PROFILE_FUNCTION();

    if (!Exists(name))
    {
        LOG_ENGINE_WARN("UniformBuffer not found: " + name);
        return;
    }
    m_UniformBufferMap[name]->SetData(dataList);
}

MCEngine::UniformBufferLibrary::UniformBufferLibrary()
{
    ENGINE_PROFILE_FUNCTION();

    // todo: check size
    AddUniformBuffer("UniformBuffer0", sizeof(glm::mat4) * 10, 0);
}

bool MCEngine::UniformBufferLibrary::Exists(const std::string &name) const
{
    return m_UniformBufferMap.find(name) != m_UniformBufferMap.end();
}
