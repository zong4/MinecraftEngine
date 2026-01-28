#include "LuaScriptSystem.hpp"

void Engine::LuaScriptSystem::Start(entt::registry &registry)
{
    auto &&view = registry.view<LuaScriptComponent>();
    for (auto &&entity : view)
    {
        auto &&luaScript = view.get<LuaScriptComponent>(entity);
        sol::load_result script = m_Lua.load_file(luaScript.ScriptPath);
        if (!script.valid())
        {
            sol::error err = script;
            LOG_ENGINE_ERROR("Failed to load Lua script: " + std::string(err.what()));
            continue;
        }
        sol::protected_function_result result = script();
        if (!result.valid())
        {
            sol::error err = result;
            LOG_ENGINE_ERROR("Error during Lua script execution: " + std::string(err.what()));
            continue;
        }
        luaScript.Instance = result;
        luaScript.Entity = Entity(entity, &registry);
    }

    // Call OnStart for each script
    for (auto &&entity : view)
    {
        auto &&luaScript = view.get<LuaScriptComponent>(entity);
        if (luaScript.Instance.valid())
        {
            sol::protected_function startFunc = luaScript.Instance["OnStart"];
            if (startFunc.valid())
            {
                sol::protected_function_result result = startFunc(luaScript.Instance);
                if (!result.valid())
                {
                    sol::error err = result;
                    LOG_ENGINE_ERROR("Error during Lua OnStart: " + std::string(err.what()));
                }
            }
        }
    }
}

void Engine::LuaScriptSystem::Update(entt::registry &registry, float deltaTime)
{
    auto &&view = registry.view<LuaScriptComponent>();
    for (auto &&entity : view)
    {
        auto &&luaScript = view.get<LuaScriptComponent>(entity);
        if (luaScript.Instance.valid())
        {
            sol::protected_function updateFunc = luaScript.Instance["OnUpdate"];
            if (updateFunc.valid())
            {
                sol::protected_function_result result = updateFunc(luaScript.Instance, deltaTime);
                if (!result.valid())
                {
                    sol::error err = result;
                    LOG_ENGINE_ERROR("Error during Lua OnUpdate: " + std::string(err.what()));
                }
            }
        }
    }
}

void Engine::LuaScriptSystem::Clear(entt::registry &registry)
{
    auto &&view = registry.view<LuaScriptComponent>();
    for (auto &&entity : view)
    {
        auto &&luaScript = view.get<LuaScriptComponent>(entity);
        if (luaScript.Instance.valid())
        {
            sol::protected_function destroyFunc = luaScript.Instance["OnDestroy"];
            if (destroyFunc.valid())
            {
                sol::protected_function_result result = destroyFunc(luaScript.Instance);
                if (!result.valid())
                {
                    sol::error err = result;
                    LOG_ENGINE_ERROR("Error during Lua OnDestroy: " + std::string(err.what()));
                }
            }
            luaScript.Instance.reset();
        }
    }
}