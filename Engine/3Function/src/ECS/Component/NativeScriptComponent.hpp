#pragma once

#include "../Entity/ScriptableEntity.hpp"

namespace Engine
{

struct NativeScriptComponent
{
    std::shared_ptr<ScriptableEntity> Instance;

public:
    NativeScriptComponent() = default;

    std::function<std::shared_ptr<ScriptableEntity>()> InstantiateScript;
    std::function<void()> DestroyScript;

public:
    // clang-format off
    template <typename T>
    // clang-format on
    void Bind(const Entity &entity)
    {
        InstantiateScript = [entity, this]() {
            Instance = std::make_shared<T>(entity);
            Instance->OnCreate();
            return Instance;
        };
        DestroyScript = [this]() {
            if (Instance)
            {
                Instance->OnDestroy();
                Instance.reset();
            }
        };
    }
};

} // namespace Engine