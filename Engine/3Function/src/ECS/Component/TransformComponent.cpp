#include "TransformComponent.hpp"

#include "RelationshipComponent.hpp"

glm::vec3 Engine::TransformComponent::GetWorldScale() const
{
    // Extract scale from the transformation matrix
    glm::vec3 scale;
    scale.x = glm::length(glm::vec3(m_TransformMatrix[0]));
    scale.y = glm::length(glm::vec3(m_TransformMatrix[1]));
    scale.z = glm::length(glm::vec3(m_TransformMatrix[2]));
    return scale;
}

const glm::quat &Engine::TransformComponent::GetRotationQuat(TransformSpace space) const
{
    if (space == TransformSpace::Local)
        return m_RotationQuat;
    else
        return m_GlobalRotationQuat;
}

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

void Engine::TransformComponent::UpdateTransformMatrix(const glm::mat4 &parentTransformMatrix,
                                                       const glm::quat &parentRotationQuat)
{
    // Compute local transformation matrices
    glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), Position);
    m_RotationQuat = glm::quat(Rotation);
    glm::mat4 rotationMatrix = glm::mat4_cast(m_RotationQuat);
    glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), Scale);
    glm::mat4 localTransformMatrix = translationMatrix * rotationMatrix * scaleMatrix;

    // Combine with parent's transformation
    m_TransformMatrix = parentTransformMatrix * localTransformMatrix;
    m_GlobalRotationQuat = parentRotationQuat * m_RotationQuat;
}
