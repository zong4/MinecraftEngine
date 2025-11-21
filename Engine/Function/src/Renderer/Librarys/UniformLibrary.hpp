#pragma once

#include <Platform.hpp>

namespace Engine
{

class UniformLibrary
{
public:
    static UniformLibrary &GetInstance();

    std::shared_ptr<UniformBuffer> GetUniform(const std::string &name);
    void AddUniform(const std::string &name, size_t size, unsigned int binding);
    void RemoveUniform(const std::string &name);
    void UpdateUniform(const std::string &name, const std::initializer_list<UniformBufferData> &dataList);

private:
    std::unordered_map<std::string, std::shared_ptr<UniformBuffer>> m_UniformMap;

private:
    UniformLibrary();
    ~UniformLibrary() = default;

    bool Exists(const std::string &name) const { return m_UniformMap.find(name) != m_UniformMap.end(); }
};

} // namespace Engine