#pragma once

#include "../Component/LuaScriptComponent.hpp"
#include <entt/entt.hpp>

namespace Engine
{

class LuaScriptSystem
{
public:
    LuaScriptSystem()
    {
        m_Lua.open_libraries(sol::lib::base, sol::lib::math, sol::lib::os, sol::lib::string, sol::lib::table);
    }
    ~LuaScriptSystem() { m_Lua.collect_garbage(); }

public:
    void Start(entt::registry &registry);
    void Update(entt::registry &registry, float deltaTime);
    void Clear(entt::registry &registry);

private:
    sol::state m_Lua;
};

} // namespace Engine