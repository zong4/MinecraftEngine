#pragma once

#include "../src/ECS/Entity/ScriptableEntity.hpp"

#include <glm/gtc/type_ptr.hpp>

namespace Engine
{

struct TagComponent
{
    std::string Tag;

public:
    TagComponent(const std::string &tag) : Tag(tag) {}
};

struct RelationshipComponent
{
public:
    RelationshipComponent(Entity parent = Entity()) : Parent(parent) {}

    // Getters
    Entity GetParent() const { return Parent; }
    const std::vector<Entity> &GetChildren() const { return m_Children; }

    // Setters
    void SetParent(Entity parent) { Parent = parent; }
    void AddChild(Entity child) { m_Children.push_back(child); }
    void RemoveChild(Entity child)
    {
        m_Children.erase(std::remove(m_Children.begin(), m_Children.end(), child), m_Children.end());
    }
    void ClearChildren() { m_Children.clear(); }

public:
    static void SetParentChild(Entity parent, Entity child)
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

private:
    Entity Parent;
    std::vector<Entity> m_Children;
};

enum class TransformSpace
{
    Local,
    Global
};

// Use euler to store
// Use radian to rotate
// Use quat to calculate
struct TransformComponent
{
    glm::vec3 Position;
    glm::vec3 Scale;

public:
    TransformComponent(const glm::vec3 &position = glm::vec3(0.0f), const glm::vec3 &rotation = glm::vec3(0.0f),
                       const glm::vec3 &scale = glm::vec3(1.0f))
        : Position(position), Scale(scale)
    {
        SetRotationEuler(rotation);
    }

    // Getters
    const glm::vec3 &GetRotationRadians() const { return m_RotationRadians; }
    glm::vec3 GetRotationEuler() const { return glm::degrees(m_RotationRadians); }
    const glm::mat4 &GetTransformMatrix() const { return m_TransformMatrix; }
    glm::vec3 GetForward(TransformSpace space) const;
    glm::vec3 GetRight(TransformSpace space) const;
    glm::vec3 GetUp(TransformSpace space) const;

    // Setters
    void SetRotationRadians(const glm::vec3 &radians);
    void SetRotationEuler(const glm::vec3 &euler);

public:
    void UpdateTransformMatrix(const glm::mat4 &parentTransformMatrix, const glm::quat &parentRotationQuat,
                               RelationshipComponent *relationship);

private:
    glm::vec3 m_RotationRadians;
    glm::quat m_RotationQuat;
    glm::quat m_GlobalRotationQuat;
    glm::mat4 m_TransformMatrix = glm::mat4(1.0f);
};

struct NativeScriptComponent
{
    std::shared_ptr<ScriptableEntity> Instance;

public:
    NativeScriptComponent() = default;

    std::function<std::shared_ptr<ScriptableEntity>()> InstantiateScript;
    std::function<void()> DestroyScript;

public:
    // clang-format off
    template <typename T>
    // clang-format on
    void Bind(const Entity &entity)
    {
        InstantiateScript = [entity, this]() {
            Instance = std::make_shared<T>(entity);
            Instance->OnCreate();
            return Instance;
        };
        DestroyScript = [this]() {
            Instance->OnDestroy();
            Instance.reset();
        };
    }
};

} // namespace Engine