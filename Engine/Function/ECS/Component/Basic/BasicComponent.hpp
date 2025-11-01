#pragma once

#include "ECS/Entity/ScriptableEntity.hpp"

namespace MCEngine
{

struct TagComponent
{
    std::string Tag;

public:
    TagComponent(const std::string &tag) : Tag(tag) {}
};

struct RelationshipComponent
{
    Entity Parent;

public:
    RelationshipComponent(Entity parent = Entity()) : Parent(parent) {}

    // Getters
    const std::vector<Entity> &GetChildren() const { return m_Children; }

    // Setters
    void AddChild(Entity child) { m_Children.push_back(child); }
    void RemoveChild(Entity child)
    {
        m_Children.erase(std::remove(m_Children.begin(), m_Children.end(), child), m_Children.end());
    }

private:
    std::vector<Entity> m_Children;
};

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
    glm::vec3 GetForward() const { return glm::normalize(m_GlobalRotationQuat * glm::vec3(0, 0, -1)); }
    glm::vec3 GetRight() const { return glm::normalize(m_GlobalRotationQuat * glm::vec3(1, 0, 0)); }
    glm::vec3 GetUp() const { return glm::normalize(m_GlobalRotationQuat * glm::vec3(0, 1, 0)); }

    // Setters
    void SetRotationRadians(const glm::vec3 &radians);
    void SetRotationEuler(const glm::vec3 &euler);
    void SetRotationQuat(const glm::quat &quat);

public:
    void UpdateTransformMatrix(const glm::mat4 &parentTransformMatrix, const glm::quat &parentRotationQuat,
                               const RelationshipComponent &relationship);

private:
    glm::vec3 m_RotationRadians;
    glm::quat m_RotationQuat;
    glm::quat m_GlobalRotationQuat;
    glm::mat4 m_TransformMatrix = glm::mat4(1.0f);
};

class NativeScriptComponent
{
public:
    NativeScriptComponent() = default;

    std::function<std::shared_ptr<ScriptableEntity>()> InstantiateScript;
    std::function<void()> DestroyScript;

public:
    // clang-format off
    template <typename T>
    // clang-format on
    void Bind()
    {
        InstantiateScript = [&]() {
            Instance = std::make_shared<T>();
            return Instance;
        };
        DestroyScript = [&]() { Instance->OnDestroy(); };
    }

public:
    std::shared_ptr<ScriptableEntity> Instance;
};

} // namespace MCEngine