#include "RelationshipComponent.hpp"

void Engine::RelationshipComponent::RemoveChild(const Entity &child)
{
    m_Children.erase(std::remove(m_Children.begin(), m_Children.end(), child), m_Children.end());
}

void Engine::RelationshipComponent::SetParentChild(const Entity &parent, const Entity &child)
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