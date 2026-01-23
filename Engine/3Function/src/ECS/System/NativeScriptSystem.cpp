#include "NativeScriptSystem.hpp"

Engine::NativeScriptSystem &Engine::NativeScriptSystem::GetInstance()
{
    static NativeScriptSystem instance;
    return instance;
}

void Engine::NativeScriptSystem::Start(entt::registry &registry)
{
    PROFILE_FUNCTION();

    // Create all scripts
    registry.view<Engine::NativeScriptComponent>().each([&](auto &&entity, auto &&nsc) {
        nsc.Instance = nsc.InstantiateScript();
        nsc.Instance->OnCreate();
    });

    // Start all scripts
    registry.view<Engine::NativeScriptComponent>().each([&](auto &&entity, auto &&nsc) { nsc.Instance->OnStart(); });
}

void Engine::NativeScriptSystem::Update(entt::registry &registry, float deltaTime)
{
    PROFILE_FUNCTION();

    // Update all scripts
    registry.view<Engine::NativeScriptComponent>().each(
        [&](auto &&entity, auto &&nsc) { nsc.Instance->OnUpdate(deltaTime); });
}

void Engine::NativeScriptSystem::Clear(entt::registry &registry)
{
    PROFILE_FUNCTION();

    // Destroy all scripts
    registry.view<Engine::NativeScriptComponent>().each([&](auto &&entity, auto &&nsc) { nsc.DestroyScript(); });
}