#include "OpenGLShader.hpp"

#include "../RendererCommand.hpp"
#include <glad/glad.h>

Engine::OpenGLShader::OpenGLShader(const std::string &vertexSource, const std::string &fragmentSource,
                                   const std::string &geometrySource)
{
    m_RendererID = glCreateProgram();

    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    CompileShader(vertexShader, vertexSource);
    glAttachShader(m_RendererID, vertexShader);

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    CompileShader(fragmentShader, fragmentSource);
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

    LOG_ENGINE_INFO("OpenGLShader program created with ID: " + std::to_string(m_RendererID));
}

Engine::OpenGLShader::~OpenGLShader() { glDeleteProgram(m_RendererID); }

void Engine::OpenGLShader::SetUniformInt(const std::string &name, int value)
{
    int location = glGetUniformLocation(m_RendererID, name.c_str());
    glUniform1i(location, value);
    RendererCommand::GetError(std::string(FUNCTION_SIGNATURE));
}

void Engine::OpenGLShader::SetUniformUInt(const std::string &name, unsigned int value)
{
    int location = glGetUniformLocation(m_RendererID, name.c_str());
    glUniform1ui(location, value);
    RendererCommand::GetError(std::string(FUNCTION_SIGNATURE));
}

void Engine::OpenGLShader::SetUniformFloat(const std::string &name, float value)
{
    int location = glGetUniformLocation(m_RendererID, name.c_str());
    glUniform1f(location, value);
    RendererCommand::GetError(std::string(FUNCTION_SIGNATURE));
}

void Engine::OpenGLShader::SetUniformVec3(const std::string &name, glm::vec3 vector3)
{
    int location = glGetUniformLocation(m_RendererID, name.c_str());
    glUniform3fv(location, 1, &vector3[0]);
    RendererCommand::GetError(std::string(FUNCTION_SIGNATURE));
}

void Engine::OpenGLShader::SetUniformVec4(const std::string &name, glm::vec4 vector4)
{
    int location = glGetUniformLocation(m_RendererID, name.c_str());
    glUniform4fv(location, 1, &vector4[0]);
    RendererCommand::GetError(std::string(FUNCTION_SIGNATURE));
}

void Engine::OpenGLShader::SetUniformMat4(const std::string &name, glm::mat4 matrix4)
{
    int location = glGetUniformLocation(m_RendererID, name.c_str());
    glUniformMatrix4fv(location, 1, GL_FALSE, &matrix4[0][0]);
    RendererCommand::GetError(std::string(FUNCTION_SIGNATURE));
}

void Engine::OpenGLShader::Bind() const { glUseProgram(m_RendererID); }

void Engine::OpenGLShader::Unbind() const { glUseProgram(0); }

void Engine::OpenGLShader::CompileShader(unsigned int shaderID, const std::string &source)
{
    const char *sourceCStr = source.c_str();
    glShaderSource(shaderID, 1, &sourceCStr, nullptr);
    glCompileShader(shaderID);

    int success;
    char infoLog[512];
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shaderID, 512, NULL, infoLog);
        LOG_ENGINE_ERROR("SHADER::COMPILATION_FAILED\n" + std::string(infoLog));
    }
}

void Engine::OpenGLShader::LinkProgram(unsigned int programID)
{
    glLinkProgram(programID);

    int success;
    char infoLog[512];
    glGetProgramiv(programID, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(programID, 512, NULL, infoLog);
        LOG_ENGINE_ERROR("SHADER::LINKING_FAILED\n" + std::string(infoLog));
    }
}
