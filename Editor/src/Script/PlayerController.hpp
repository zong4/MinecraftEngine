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
    float m_JumpStrength = 10.0f;
    glm::vec3 m_Offset = glm::vec3(0.0f, 5.0f, 12.0f);
    glm::vec3 m_CameraRotation = glm::vec3(-30.0f, 0.0f, 0.0f);
    Engine::TransformComponent *m_Transform = nullptr;
    Engine::RigidBodyComponent *m_RigidBody = nullptr;
    Engine::AudioComponent *m_WalkAudio = nullptr;
};

} // namespace Editor