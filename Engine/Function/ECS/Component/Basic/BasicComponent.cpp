#include "BasicComponent.hpp"

void MCEngine::TransformComponent::SetRotationRadians(const glm::vec3 &radians)
{
    m_RotationRadians = radians;
    m_RotationQuat = glm::quat(radians);
}

void MCEngine::TransformComponent::SetRotationEuler(const glm::vec3 &euler)
{
    m_RotationRadians = glm::radians(euler);
    m_RotationQuat = glm::quat(m_RotationRadians);
}

void MCEngine::TransformComponent::UpdateTransformMatrix(const glm::mat4 &parentTransformMatrix,
                                                         const glm::quat &parentRotationQuat,
                                                         const RelationshipComponent &relationship)
{
    ENGINE_PROFILE_FUNCTION();

    glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), Position);
    glm::mat4 rotationMatrix = glm::mat4_cast(m_RotationQuat);
    glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), Scale);

    m_GlobalRotationQuat = parentRotationQuat * m_RotationQuat;
    glm::mat4 localTransformMatrix = translationMatrix * rotationMatrix * scaleMatrix;
    m_TransformMatrix = parentTransformMatrix * localTransformMatrix;
    for (auto &&child : relationship.GetChildren())
    {
        child.GetComponent<TransformComponent>().UpdateTransformMatrix(m_TransformMatrix, m_GlobalRotationQuat,
                                                                       child.GetComponent<RelationshipComponent>());
    }
}
