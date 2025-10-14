#include "Shader.hpp"

#include <glad/glad.h>

MCEngine::Shader::Shader(const std::string &vertexSource, const std::string &fragmentSource,
                         const std::string &geometrySource)
{
    ENGINE_PROFILE_FUNCTION();

    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    CompileShader(vertexShader, vertexSource);

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    CompileShader(fragmentShader, fragmentSource);

    m_RendererID = glCreateProgram();
    glAttachShader(m_RendererID, vertexShader);
    glAttachShader(m_RendererID, fragmentShader);

    unsigned int geometryShader = 0;
    if (!geometrySource.empty())
    {
        geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
        CompileShader(geometryShader, geometrySource);

        glAttachShader(m_RendererID, geometryShader);
    }
    LinkProgram(m_RendererID);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    if (!geometrySource.empty())
    {
        glDeleteShader(geometryShader);
    }

    LOG_ENGINE_INFO("Shader program created with ID: " + std::to_string(m_RendererID));
}

MCEngine::Shader::~Shader() { glDeleteProgram(m_RendererID); }

void MCEngine::Shader::SetUniformInt(const std::string &name, int value)
{
    ENGINE_PROFILE_FUNCTION();

    int location = glGetUniformLocation(m_RendererID, name.c_str());
    glUniform1i(location, value);
    RendererCommand::GetError(std::string(FUNCTION_SIGNATURE));
}

void MCEngine::Shader::SetUniformUInt(const std::string &name, unsigned int value)
{
    ENGINE_PROFILE_FUNCTION();

    int location = glGetUniformLocation(m_RendererID, name.c_str());
    glUniform1ui(location, value);
    RendererCommand::GetError(std::string(FUNCTION_SIGNATURE));
}

void MCEngine::Shader::SetUniformFloat(const std::string &name, float value)
{
    ENGINE_PROFILE_FUNCTION();

    int location = glGetUniformLocation(m_RendererID, name.c_str());
    glUniform1f(location, value);
    RendererCommand::GetError(std::string(FUNCTION_SIGNATURE));
}

void MCEngine::Shader::SetUniformVec3(const std::string &name, glm::vec3 vector3)
{
    ENGINE_PROFILE_FUNCTION();

    int location = glGetUniformLocation(m_RendererID, name.c_str());
    glUniform3fv(location, 1, &vector3[0]);
    RendererCommand::GetError(std::string(FUNCTION_SIGNATURE));
}

void MCEngine::Shader::SetUniformVec4(const std::string &name, glm::vec4 vector4)
{
    ENGINE_PROFILE_FUNCTION();

    int location = glGetUniformLocation(m_RendererID, name.c_str());
    glUniform4fv(location, 1, &vector4[0]);
    RendererCommand::GetError(std::string(FUNCTION_SIGNATURE));
}

void MCEngine::Shader::SetUniformMat4(const std::string &name, glm::mat4 matrix4)
{
    ENGINE_PROFILE_FUNCTION();

    int location = glGetUniformLocation(m_RendererID, name.c_str());
    glUniformMatrix4fv(location, 1, GL_FALSE, &matrix4[0][0]);
    RendererCommand::GetError(std::string(FUNCTION_SIGNATURE));
}

void MCEngine::Shader::Bind() const { glUseProgram(m_RendererID); }

void MCEngine::Shader::Unbind() const { glUseProgram(0); }

void MCEngine::Shader::CompileShader(unsigned int shaderID, const std::string &source)
{
    ENGINE_PROFILE_FUNCTION();

    const char *sourceCStr = source.c_str();
    glShaderSource(shaderID, 1, &sourceCStr, nullptr);
    glCompileShader(shaderID);

    int success;
    char infoLog[512];
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shaderID, 512, NULL, infoLog);
        LOG_ENGINE_ERROR("ERROR::SHADER::COMPILATION_FAILED\n" + std::string(infoLog));
    }
}

void MCEngine::Shader::LinkProgram(unsigned int programID)
{
    ENGINE_PROFILE_FUNCTION();

    glLinkProgram(programID);

    int success;
    char infoLog[512];
    glGetProgramiv(programID, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(programID, 512, NULL, infoLog);
        LOG_ENGINE_ERROR("ERROR::SHADER::PROGRAM::LINKING_FAILED\n" + std::string(infoLog));
    }
}
