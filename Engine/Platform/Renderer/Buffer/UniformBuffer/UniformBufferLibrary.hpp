#include "UniformBuffer.hpp"

namespace MCEngine
{

class UniformBufferLibrary
{
public:
    static UniformBufferLibrary &GetInstance();

    std::shared_ptr<UniformBuffer> GetUniformBuffer(const std::string &name);
    void AddUniformBuffer(const std::string &name, size_t size, unsigned int binding);
    void RemoveUniformBuffer(const std::string &name);
    void UpdateUniformBuffer(const std::string &name, const std::initializer_list<UniformBufferData> &dataList);

private:
    std::unordered_map<std::string, std::shared_ptr<UniformBuffer>> m_UniformBufferMap;

private:
    UniformBufferLibrary();
    ~UniformBufferLibrary() = default;

    bool Exists(const std::string &name) const;
};

} // namespace MCEngine