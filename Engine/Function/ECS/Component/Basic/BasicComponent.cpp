#include "BasicComponent.hpp"

void MCEngine::RelationshipComponent::RemoveChild(Entity child)
{
    ENGINE_PROFILE_FUNCTION();

    m_Children.erase(std::remove(m_Children.begin(), m_Children.end(), child), m_Children.end());
}

MCEngine::TransformComponent::TransformComponent(const glm::vec3 &position, const glm::vec3 &rotation,
                                                 const glm::vec3 &scale)
    : m_Position(position), m_Rotation(rotation), m_Scale(scale)
{
    m_RotationQuat = glm::quat(glm::radians(rotation));
    LOG_ENGINE_TRACE("Transform Component created with Position: " + Math::ToString(m_Position) +
                     ", Rotation: " + Math::ToString(m_Rotation) + ", Scale: " + Math::ToString(m_Scale));
}

void MCEngine::TransformComponent::SetRotationQuat(const glm::quat &quat)
{
    m_RotationQuat = quat;
    m_Rotation = glm::degrees(glm::eulerAngles(quat));
}

void MCEngine::TransformComponent::SetRotationEuler(const glm::vec3 &euler)
{
    m_Rotation = euler;
    m_RotationQuat = glm::quat(glm::radians(euler));
}

void MCEngine::TransformComponent::UpdateTransformMatrix(const glm::mat4 &parentTransformMatrix,
                                                         const glm::quat &parentRotation,
                                                         const RelationshipComponent &relationship)
{
    ENGINE_PROFILE_FUNCTION();

    glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), m_Position);
    glm::mat4 rotationMatrix = glm::mat4_cast(m_RotationQuat);
    glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), m_Scale);

    m_GlobalRotationQuat = parentRotation * m_RotationQuat;
    glm::mat4 localTransformMatrix = translationMatrix * rotationMatrix * scaleMatrix;
    m_TransformMatrix = parentTransformMatrix * localTransformMatrix;
    for (auto &&child : relationship.GetChildren())
    {
        child.GetComponent<TransformComponent>().UpdateTransformMatrix(m_TransformMatrix, m_GlobalRotationQuat,
                                                                       child.GetComponent<RelationshipComponent>());
    }
}
