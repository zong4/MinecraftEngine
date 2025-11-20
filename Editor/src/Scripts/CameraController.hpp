#pragma once

#include <Function.hpp>

namespace Editor
{

class CameraController : public MCEngine::ScriptableEntity
{
public:
    CameraController(MCEngine::Entity entity) : MCEngine::ScriptableEntity(entity) {}
    ~CameraController() override = default;

public:
    void OnCreate() override;
    void OnUpdate(float deltaTime) override;

private:
    float m_MoveSpeed = 0.2f;
    float m_RotateSpeed = 2.0f;
    float m_ZoomSpeed = 1.5f;
    MCEngine::TransformComponent *m_Transform = nullptr;
};

} // namespace Editor