#include "UniformLibrary.hpp"

Engine::UniformLibrary &Engine::UniformLibrary::GetInstance()
{
    static UniformLibrary instance;
    return instance;
}

std::shared_ptr<Engine::UniformBuffer> Engine::UniformLibrary::GetUniform(const std::string &name)
{
    if (!Exists(name))
    {
        LOG_ENGINE_ERROR("Uniform not found: " + name);
        return nullptr;
    }
    return m_UniformMap[name];
}

void Engine::UniformLibrary::AddUniform(const std::string &name, size_t size, unsigned int binding)
{
    if (Exists(name))
    {
        LOG_ENGINE_WARN("Uniform already exists: " + name);
        return;
    }
    m_UniformMap[name] = UniformBuffer::Create(size, binding);
    LOG_ENGINE_INFO("Uniform added: " + name);
}

void Engine::UniformLibrary::RemoveUniform(const std::string &name)
{
    if (!Exists(name))
    {
        LOG_ENGINE_WARN("Uniform not found: " + name);
        return;
    }
    m_UniformMap.erase(name);
    LOG_ENGINE_INFO("Uniform removed: " + name);
}

void Engine::UniformLibrary::UpdateUniform(const std::string &name,
                                           const std::initializer_list<UniformBufferData> &dataList)
{
    if (!Exists(name))
    {
        LOG_ENGINE_ERROR("Uniform not found: " + name);
        return;
    }
    m_UniformMap[name]->SetData(dataList);
}

Engine::UniformLibrary::UniformLibrary() { AddUniform("UniformBuffer0", sizeof(glm::mat4) * 10, 0); }
