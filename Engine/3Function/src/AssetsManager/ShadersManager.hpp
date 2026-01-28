#pragma once

#include "../Application/FileSystem.hpp"

namespace Engine
{

class ShadersManager
{
public:
    static ShadersManager &GetInstance();

    // Getters
    std::string GetName(const std::shared_ptr<Shader> &shader) const;
    std::shared_ptr<Shader> GetShader(const std::string &name);
    std::shared_ptr<Shader> GetDefaultShader() const { return m_ShadersMap.at("BlinnPhong"); }

    // Setters
    void AddShader(const std::string &name, const std::shared_ptr<Shader> &shader);
    std::shared_ptr<Shader> LoadShader(const std::string &name, const std::string &vertexSource,
                                       const std::string &fragmentSource, const std::string &geometrySource = "");

private:
    std::unordered_map<std::string, std::shared_ptr<Shader>> m_ShadersMap;

private:
    ShadersManager();
    ~ShadersManager() = default;

    bool Exists(const std::string &name) const { return m_ShadersMap.find(name) != m_ShadersMap.end(); }
};

} // namespace Engine