#pragma once

#include "pch.hpp"

namespace MCEngine
{

class Entity
{
public:
    Entity() = default;
    Entity(entt::entity handle, entt::registry *registry) : m_Handle(handle), m_Registry(registry) {}
    virtual ~Entity() { m_Registry = nullptr; }

    // Operators
    operator bool() const { return m_Handle != entt::null && m_Registry && m_Registry->valid(m_Handle); }
    operator uint32_t() const { return (uint32_t)m_Handle; }
    operator entt::entity() const { return m_Handle; }
    bool operator==(const Entity &other) const { return m_Handle == other.m_Handle && m_Registry == other.m_Registry; }
    bool operator!=(const Entity &other) const { return m_Handle != other.m_Handle || m_Registry != other.m_Registry; }

    // Getters
    entt::entity GetHandle() const { return m_Handle; }
    entt::registry &GetRegistry() const { return *m_Registry; }

public:
    // clang-format off
    template <typename T>
    // clang-format on
    bool HasComponent() const
    {
        if (!(*this))
            return false;
        return m_Registry->all_of<T>(m_Handle);
    }
    // clang-format off
    template <typename T>
    // clang-format on
    T &GetComponent() const
    {
        if (!HasComponent<T>())
            LOG_ENGINE_ERROR("Entity does not have component!");
        return m_Registry->get<T>(m_Handle);
    }
    // clang-format off
    template <typename T, typename... Args>
    // clang-format on
    T &AddComponent(Args &&...args)
    {
        if (HasComponent<T>())
            LOG_ENGINE_ERROR("Entity already has component!");
        return m_Registry->emplace<T>(m_Handle, std::forward<Args>(args)...);
    }
    // clang-format off
    template <typename T>
    // clang-format on
    void RemoveComponent()
    {
        if (!HasComponent<T>())
            LOG_ENGINE_ERROR("Entity does not have component!");
        m_Registry->remove<T>(m_Handle);
    }

protected:
    entt::entity m_Handle = entt::null;
    entt::registry *m_Registry = nullptr;
};

} // namespace MCEngine
