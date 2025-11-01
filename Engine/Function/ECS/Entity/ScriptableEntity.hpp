#pragma once

#include "Entity.hpp"

namespace MCEngine
{

class ScriptableEntity
{
public:
    ScriptableEntity(const Entity &entity) : m_Entity(entity) {}
    virtual ~ScriptableEntity() = default;

public:
    virtual void OnCreate() {}
    virtual void OnStart() {}
    virtual void OnUpdate(float deltaTime) = 0;
    virtual void OnDestroy() {}

    // clang-format off
    template <typename T>
    // clang-format on
    bool HasComponent() const
    {
        return m_Entity.HasComponent<T>();
    }
    // clang-format off
    template <typename T>
    // clang-format on
    T &GetComponent()
    {
        return m_Entity.GetComponent<T>();
    }
    // clang-format off
    template <typename T, typename... Args>
    // clang-format on
    void AddComponent(Args &&...args)
    {
        return m_Entity.AddComponent<T>(std::forward<Args>(args)...);
    }
    // clang-format off
    template <typename T>
    // clang-format on
    void RemoveComponent()
    {
        m_Entity.RemoveComponent<T>();
    }

private:
    Entity m_Entity;
};

} // namespace MCEngine