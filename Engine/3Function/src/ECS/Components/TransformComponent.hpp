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

// Use euler to show and init
// Use radian to store
// Use quat to calculate
struct TransformComponent
{
    glm::vec3 Position;
    glm::vec3 Scale;

public:
    TransformComponent(const glm::vec3 &position = glm::vec3(0.0f), const glm::vec3 &rotation = glm::vec3(0.0f),
                       const glm::vec3 &scale = glm::vec3(1.0f));

    // Getters
    const glm::vec3 &GetRotationRadians() const { return m_RotationRadians; }
    glm::vec3 GetRotationEuler() const { return glm::degrees(m_RotationRadians); }
    const glm::quat &GetRotationQuat(TransformSpace space) const;
    const glm::mat4 &GetTransformMatrix() const { return m_TransformMatrix; }
    glm::vec3 GetForward(TransformSpace space) const;
    glm::vec3 GetRight(TransformSpace space) const;
    glm::vec3 GetUp(TransformSpace space) const;

    // Setters
    void SetRotationRadians(const glm::vec3 &radians) { m_RotationRadians = radians; }
    void SetRotationEuler(const glm::vec3 &euler) { m_RotationRadians = glm::radians(euler); }

public:
    void UpdateTransformMatrix(const glm::mat4 &parentTransformMatrix, const glm::quat &parentRotationQuat,
                               RelationshipComponent *relationship);

private:
    glm::vec3 m_RotationRadians;
    glm::quat m_RotationQuat;
    glm::quat m_GlobalRotationQuat = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    glm::mat4 m_TransformMatrix = glm::mat4(1.0f);
};

} // namespace Engine