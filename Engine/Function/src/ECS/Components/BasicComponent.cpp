#include "BasicComponent.hpp"

void Engine::RelationshipComponent::RemoveChild(Entity child)
{
    m_Children.erase(std::remove(m_Children.begin(), m_Children.end(), child), m_Children.end());
}

void Engine::RelationshipComponent::SetParentChild(Entity parent, Entity child)
{
    if (auto &&childRelationship = child.GetComponent<RelationshipComponent>())
    {
        // Remove from old parent
        if (auto &&oldParentRelationship = childRelationship->GetParent().GetComponent<RelationshipComponent>())
            oldParentRelationship->RemoveChild(child);

        // Set new parent
        childRelationship->SetParent(parent);

        // Add to new parent's children list
        if (auto &&parentRelationship = parent.GetComponent<RelationshipComponent>())
            parentRelationship->AddChild(child);
    }
}

Engine::TransformComponent::TransformComponent(const glm::vec3 &position, const glm::vec3 &rotation,
                                               const glm::vec3 &scale)
    : Position(position), Scale(scale)
{
    SetRotationEuler(rotation);
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
                                                       const glm::quat &parentRotationQuat,
                                                       RelationshipComponent *relationship)
{
    PROFILE_FUNCTION();

    // Compute local transformation matrices
    glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), Position);
    m_RotationQuat = glm::quat(m_RotationRadians);
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
