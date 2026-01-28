#include "ScriptsSystem.hpp"

Engine::ScriptsSystem::ScriptsSystem()
{
    m_Lua.open_libraries(sol::lib::base, sol::lib::math, sol::lib::os, sol::lib::string, sol::lib::table);
}

Engine::ScriptsSystem::~ScriptsSystem() { m_Lua.collect_garbage(); }

void Engine::ScriptsSystem::Delete(const Entity &entity)
{
    PROFILE_FUNCTION();

    // Delete native script
    if (auto &&nativeScript = entity.GetComponent<NativeScriptComponent>())
        nativeScript->DestroyScript();

    // Delete Lua script
    if (auto &&luaScript = entity.GetComponent<LuaScriptComponent>())
    {
        if (luaScript->Instance.valid())
        {
            sol::protected_function destroyFunc = luaScript->Instance["OnDestroy"];
            if (destroyFunc.valid())
            {
                sol::protected_function_result result = destroyFunc(luaScript->Instance);
                if (!result.valid())
                {
                    sol::error err = result;
                    LOG_ENGINE_ERROR("Error during Lua OnDestroy: " + std::string(err.what()));
                }
            }

            // Clear the instance
            luaScript->Instance.reset();
        }
    }
}

void Engine::ScriptsSystem::Init(entt::registry &registry)
{
    PROFILE_FUNCTION();

    Create(registry);
    Start(registry);
}

void Engine::ScriptsSystem::Update(entt::registry &registry, float deltaTime)
{
    PROFILE_FUNCTION();

    // Update all native scripts
    registry.view<Engine::NativeScriptComponent>().each(
        [&](auto &&entity, auto &&nsc) { nsc.Instance->OnUpdate(deltaTime); });

    // Update all Lua scripts
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

void Engine::ScriptsSystem::Shutdown(entt::registry &registry)
{
    PROFILE_FUNCTION();

    // Destroy all native scripts
    registry.view<Engine::NativeScriptComponent>().each([&](auto &&entity, auto &&nsc) { nsc.DestroyScript(); });

    // Destroy all Lua scripts
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

            // Clear the instance
            luaScript.Instance.reset();
        }
    }
}

void Engine::ScriptsSystem::Create(entt::registry &registry)
{
    PROFILE_FUNCTION();

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

    // Create all native scripts
    registry.view<Engine::NativeScriptComponent>().each([&](auto &&entity, auto &&nsc) {
        nsc.Instance = nsc.InstantiateScript();
        nsc.Instance->OnCreate();
    });

    // Create all Lua scripts
    for (auto &&entity : view)
    {
        auto &&luaScript = view.get<LuaScriptComponent>(entity);
        if (luaScript.Instance.valid())
        {
            sol::protected_function createFunc = luaScript.Instance["OnCreate"];
            if (createFunc.valid())
            {
                sol::protected_function_result result = createFunc(luaScript.Instance);
                if (!result.valid())
                {
                    sol::error err = result;
                    LOG_ENGINE_ERROR("Error during Lua OnCreate: " + std::string(err.what()));
                }
            }
        }
    }
}

void Engine::ScriptsSystem::Start(entt::registry &registry)
{
    PROFILE_FUNCTION();

    // Start all native scripts
    registry.view<Engine::NativeScriptComponent>().each([&](auto &&entity, auto &&nsc) { nsc.Instance->OnStart(); });

    // Start all Lua scripts
    auto &&view = registry.view<LuaScriptComponent>();
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