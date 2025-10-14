#include "UniformBuffer.hpp"

#include <glad/glad.h>

MCEngine::UniformBuffer::UniformBuffer(size_t size, unsigned int binding) : m_Binding(binding)
{
    ENGINE_PROFILE_FUNCTION();

    glGenBuffers(1, &m_RendererID);
    glBindBuffer(GL_UNIFORM_BUFFER, m_RendererID);
    glBufferData(GL_UNIFORM_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, m_Binding, m_RendererID);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    RendererCommand::GetError(std::string(FUNCTION_SIGNATURE));

    LOG_ENGINE_INFO("UniformBuffer created with ID: " + std::to_string(m_RendererID) +
                    ", Size: " + std::to_string(size) + ", Binding: " + std::to_string(m_Binding));
}

MCEngine::UniformBuffer::~UniformBuffer()
{
    ENGINE_PROFILE_FUNCTION();

    glDeleteBuffers(1, &m_RendererID);
}

void MCEngine::UniformBuffer::Bind() const
{
    ENGINE_PROFILE_FUNCTION();

    glBindBuffer(GL_UNIFORM_BUFFER, m_RendererID);
}

void MCEngine::UniformBuffer::Unbind() const
{
    ENGINE_PROFILE_FUNCTION();

    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void MCEngine::UniformBuffer::SetData(const std::initializer_list<UniformBufferData> &dataList)
{
    ENGINE_PROFILE_FUNCTION();

    Bind();
    for (const auto &data : dataList)
    {
        glBufferSubData(GL_UNIFORM_BUFFER, data.Offset, data.Size, data.Data);
        RendererCommand::GetError(std::string(FUNCTION_SIGNATURE));
    }
    Unbind();
}
