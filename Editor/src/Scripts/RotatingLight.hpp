#pragma once

#include <Function.hpp>

namespace Editor
{

class RotatingLight : public Engine::ScriptableEntity
{
public:
    RotatingLight(const Engine::Entity &entity) : Engine::ScriptableEntity(entity) {}
    ~RotatingLight() override = default;

    static bool IsRotationEnabled() { return s_RotationEnabled; }
    static void SetRotationEnabled(bool enabled) { s_RotationEnabled = enabled; }

public:
    void OnCreate() override;
    void OnUpdate(float deltaTime) override;

private:
    static bool s_RotationEnabled;
    float m_RotationSpeed = 30.0f; // degrees per second
    Engine::TransformComponent *m_Transform = nullptr;
};

} // namespace Editor
