#pragma once

#include <Function.hpp>

namespace Editor
{

class PlayerController : public Engine::ScriptableEntity
{
public:
    PlayerController(const Engine::Entity &entity) : Engine::ScriptableEntity(entity) {}
    ~PlayerController() override = default;

public:
    void OnCreate() override;
    void OnUpdate(float deltaTime) override;

private:
    float m_MoveSpeed = 10.0f;
    Engine::TransformComponent *m_Transform = nullptr;
    Engine::RigidBodyComponent *m_RigidBody = nullptr;
    Engine::SoundComponent *m_Sound = nullptr;
};

} // namespace Editor