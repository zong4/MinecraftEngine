#pragma once

#include "../Component/NativeScriptComponent.hpp"
#include <entt/entt.hpp>

namespace Engine
{

class NativeScriptSystem
{

public:
    static NativeScriptSystem &GetInstance()
    {
        static NativeScriptSystem instance;
        return instance;
    }

public:
    void Start(entt::registry &registry)
    {
        PROFILE_FUNCTION();

        // Create all scripts
        registry.view<Engine::NativeScriptComponent>().each([&](auto &&entity, auto &&nsc) {
            nsc.Instance = nsc.InstantiateScript();
            nsc.Instance->OnCreate();
        });

        // Start all scripts
        registry.view<Engine::NativeScriptComponent>().each(
            [&](auto &&entity, auto &&nsc) { nsc.Instance->OnStart(); });
    }
    void Update(entt::registry &registry, float deltaTime)
    {
        PROFILE_FUNCTION();

        // Update all scripts
        registry.view<Engine::NativeScriptComponent>().each(
            [&](auto &&entity, auto &&nsc) { nsc.Instance->OnUpdate(deltaTime); });
    }
    void Clear(entt::registry &registry)
    {
        PROFILE_FUNCTION();

        // Destroy all scripts
        registry.view<Engine::NativeScriptComponent>().each([&](auto &&entity, auto &&nsc) { nsc.DestroyScript(); });
    }

private:
    NativeScriptSystem() = default;
    ~NativeScriptSystem() = default;
};

} // namespace Engine