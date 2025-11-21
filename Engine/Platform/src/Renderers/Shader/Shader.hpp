#pragma once

#include <Core.hpp>

namespace Engine
{

class Shader
{
public:
    static std::shared_ptr<Shader> Create(const std::string &vertexSource, const std::string &fragmentSource,
                                          const std::string &geometrySource = "");

    // Getters
    unsigned int GetRendererID() const { return m_RendererID; }

    // Setters
    virtual void SetUniformInt(const std::string &name, int value) = 0;
    virtual void SetUniformUInt(const std::string &name, unsigned int value) = 0;
    virtual void SetUniformFloat(const std::string &name, float value) = 0;
    virtual void SetUniformVec3(const std::string &name, glm::vec3 vector3) = 0;
    virtual void SetUniformVec4(const std::string &name, glm::vec4 vector4) = 0;
    virtual void SetUniformMat4(const std::string &name, glm::mat4 matrix4) = 0;

public:
    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;

protected:
    unsigned int m_RendererID = 0;

protected:
    Shader() = default;
    virtual ~Shader() = default;

protected:
    virtual void CompileShader(unsigned int shaderID, const std::string &source) = 0;
    virtual void LinkProgram(unsigned int programID) = 0;
};

} // namespace Engine
