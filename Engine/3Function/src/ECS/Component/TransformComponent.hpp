#pragma once

#include <Platform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Engine
{

enum class TransformSpace
{
    Local,
    Global
};

struct RelationshipComponent;

struct TransformComponent
{
    glm::vec3 Position;
    glm::vec3 Rotation; // Use radian to store
    glm::vec3 Scale;

public:
    TransformComponent(const glm::vec3 &position = glm::vec3(0.0f),
                       const glm::vec3 &rotation = glm::vec3(0.0f), // Use euler to show and init
                       const glm::vec3 &scale = glm::vec3(1.0f))
        : Position(position), Rotation(glm::radians(rotation)), Scale(scale)
    {
    }

    // Getters
    glm::vec3 GetRotationEuler() const { return glm::degrees(Rotation); }
    const glm::quat &GetRotationQuat(TransformSpace space) const;
    const glm::mat4 &GetTransformMatrix() const { return m_TransformMatrix; }
    glm::vec3 GetForward(TransformSpace space) const;
    glm::vec3 GetRight(TransformSpace space) const;
    glm::vec3 GetUp(TransformSpace space) const;

    // Setters
    void SetRotationEuler(const glm::vec3 &euler) { Rotation = glm::radians(euler); }

public:
    void UpdateTransformMatrix(const glm::mat4 &parentTransformMatrix, const glm::quat &parentRotationQuat,
                               RelationshipComponent *relationship);

private:
    glm::quat m_RotationQuat; // Use quat to calculate
    glm::quat m_GlobalRotationQuat = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    glm::mat4 m_TransformMatrix = glm::mat4(1.0f);
};

} // namespace Engine