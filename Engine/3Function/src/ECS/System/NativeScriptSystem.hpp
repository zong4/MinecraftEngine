#pragma once

#include "../Component/NativeScriptComponent.hpp"
#include <entt/entt.hpp>

namespace Engine
{

class NativeScriptSystem
{

public:
    static NativeScriptSystem &GetInstance();

public:
    void Start(entt::registry &registry);
    void Update(entt::registry &registry, float deltaTime);
    void Clear(entt::registry &registry);

private:
    NativeScriptSystem() = default;
    ~NativeScriptSystem() = default;
};

} // namespace Engine