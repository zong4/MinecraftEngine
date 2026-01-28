#pragma once

#include "../Component/ScriptsComponent.hpp"
#include <entt/entt.hpp>

namespace Engine
{

class ScriptsSystem
{
public:
    ScriptsSystem();
    ~ScriptsSystem();

    // Setters
    void Delete(const Entity &entity);

public:
    void Init(entt::registry &registry);
    void Update(entt::registry &registry, float deltaTime);
    void Shutdown(entt::registry &registry);

private:
    sol::state m_Lua;

private:
    void Create(entt::registry &registry);
    void Start(entt::registry &registry);
};

} // namespace Engine