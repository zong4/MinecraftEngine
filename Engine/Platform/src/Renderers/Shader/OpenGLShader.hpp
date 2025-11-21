#pragma once

#include "Shader.hpp"

namespace Engine
{

class OpenGLShader : public Shader
{
public:
    OpenGLShader(const std::string &vertexSource, const std::string &fragmentSource,
                 const std::string &geometrySource = "");
    virtual ~OpenGLShader() override;

    // Setters
    void SetUniformInt(const std::string &name, int value) override;
    void SetUniformUInt(const std::string &name, unsigned int value) override;
    void SetUniformFloat(const std::string &name, float value) override;
    void SetUniformVec3(const std::string &name, glm::vec3 vector3) override;
    void SetUniformVec4(const std::string &name, glm::vec4 vector4) override;
    void SetUniformMat4(const std::string &name, glm::mat4 matrix4) override;

public:
    void Bind() const override;
    void Unbind() const override;

protected:
    void CompileShader(unsigned int shaderID, const std::string &source) override;
    void LinkProgram(unsigned int programID) override;
};

} // namespace Engine
