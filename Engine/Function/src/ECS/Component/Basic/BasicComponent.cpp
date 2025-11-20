#include "BasicComponent.hpp"

glm::vec3 Engine::TransformComponent::GetForward(TransformSpace space) const
{
    if (space == TransformSpace::Local)
        return m_RotationQuat * glm::vec3(0.0f, 0.0f, -1.0f);
    else
        return m_GlobalRotationQuat * glm::vec3(0.0f, 0.0f, -1.0f);
}

glm::vec3 Engine::TransformComponent::GetRight(TransformSpace space) const
{
    if (space == TransformSpace::Local)
        return m_RotationQuat * glm::vec3(1.0f, 0.0f, 0.0f);
    else
        return m_GlobalRotationQuat * glm::vec3(1.0f, 0.0f, 0.0f);
}

glm::vec3 Engine::TransformComponent::GetUp(TransformSpace space) const
{
    if (space == TransformSpace::Local)
        return m_RotationQuat * glm::vec3(0.0f, 1.0f, 0.0f);
    else
        return m_GlobalRotationQuat * glm::vec3(0.0f, 1.0f, 0.0f);
}

void Engine::TransformComponent::SetRotationRadians(const glm::vec3 &radians)
{
    m_RotationRadians = radians;
    m_RotationQuat = glm::quat(radians);
}

void Engine::TransformComponent::SetRotationEuler(const glm::vec3 &euler)
{
    m_RotationRadians = glm::radians(euler);
    m_RotationQuat = glm::quat(m_RotationRadians);
}

void Engine::TransformComponent::UpdateTransformMatrix(const glm::mat4 &parentTransformMatrix,
                                                       const glm::quat &parentRotationQuat,
                                                       RelationshipComponent *relationship)
{
    ENGINE_PROFILE_FUNCTION();

    // Compute local transformation matrices
    glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), Position);
    glm::mat4 rotationMatrix = glm::mat4_cast(m_RotationQuat);
    glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), Scale);
    glm::mat4 localTransformMatrix = translationMatrix * rotationMatrix * scaleMatrix;

    // Combine with parent's transformation
    m_TransformMatrix = parentTransformMatrix * localTransformMatrix;
    m_GlobalRotationQuat = parentRotationQuat * m_RotationQuat;
    for (auto &&child : relationship->GetChildren())
    {
        if (auto &&relationshipChild = child.GetComponent<RelationshipComponent>())
            child.GetComponent<TransformComponent>()->UpdateTransformMatrix(m_TransformMatrix, m_GlobalRotationQuat,
                                                                            relationshipChild);
    }
}
