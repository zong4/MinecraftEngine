#include "BasicComponent.hpp"

void MCEngine::TransformComponent::SetRotationQuat(const glm::quat &quat)
{
    m_RotationQuat = quat;
    Rotation = glm::degrees(glm::eulerAngles(quat));
}

void MCEngine::TransformComponent::SetRotationEuler(const glm::vec3 &euler)
{
    Rotation = euler;
    m_RotationQuat = glm::quat(glm::radians(euler));
}

void MCEngine::TransformComponent::UpdateTransformMatrix(const glm::mat4 &parentTransformMatrix,
                                                         const glm::quat &parentRotation,
                                                         const RelationshipComponent &relationship)
{
    ENGINE_PROFILE_FUNCTION();

    glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), Position);
    glm::mat4 rotationMatrix = glm::mat4_cast(m_RotationQuat);
    glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), Scale);

    m_GlobalRotationQuat = parentRotation * m_RotationQuat;
    glm::mat4 localTransformMatrix = translationMatrix * rotationMatrix * scaleMatrix;
    m_TransformMatrix = parentTransformMatrix * localTransformMatrix;
    for (auto &&child : relationship.GetChildren())
    {
        child.GetComponent<TransformComponent>().UpdateTransformMatrix(m_TransformMatrix, m_GlobalRotationQuat,
                                                                       child.GetComponent<RelationshipComponent>());
    }
}
