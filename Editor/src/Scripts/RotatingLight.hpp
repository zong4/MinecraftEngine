#pragma once

#include <Function.hpp>

namespace Editor
{

class RotatingLight : public Engine::ScriptableEntity
{
public:
    RotatingLight(Engine::Entity entity) : Engine::ScriptableEntity(entity) {}
    ~RotatingLight() override = default;

public:
    void OnCreate() override;
    void OnUpdate(float deltaTime) override;

    static void SetRotationEnabled(bool enabled);
    static bool IsRotationEnabled();

private:
    float m_RotationSpeed = 30.0f; // degrees per second
    Engine::TransformComponent *m_Transform = nullptr;
    static bool s_RotationEnabled;
};

} // namespace Editor


