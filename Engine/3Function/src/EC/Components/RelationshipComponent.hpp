#pragma once

#include "../Entity/Entity.hpp"

namespace Engine
{

struct RelationshipComponent
{
    Entity Parent;

public:
    RelationshipComponent(const Entity &parent = Entity()) : Parent(parent) {}

    // Getters
    const std::vector<Entity> &GetChildren() const { return m_Children; }

    // Setters
    void AddChild(const Entity &child) { m_Children.push_back(child); }
    void RemoveChild(const Entity &child);
    void ClearChildren() { m_Children.clear(); }
    static void SetParentChild(const Entity &parent, const Entity &child);

private:
    std::vector<Entity> m_Children;
};

} // namespace Engine