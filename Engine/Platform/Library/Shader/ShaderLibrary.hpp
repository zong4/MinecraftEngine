#pragma once

#include "Renderer/Shader/Shader.hpp"

namespace MCEngine
{

class ShaderLibrary
{
public:
    static ShaderLibrary &GetInstance();

    std::string GetName(const std::shared_ptr<Shader> &shader) const;
    std::shared_ptr<Shader> GetShader(const std::string &name);
    void AddShader(const std::string &name, const std::shared_ptr<Shader> &shader);
    std::shared_ptr<Shader> LoadShader(const std::string &name, const std::string &vertexSource,
                                       const std::string &fragmentSource, const std::string &geometrySource = "");

private:
    std::unordered_map<std::string, std::shared_ptr<Shader>> m_Shaders;

private:
    ShaderLibrary();
    ~ShaderLibrary() = default;

    bool Exists(const std::string &name) const;
};

} // namespace MCEngine