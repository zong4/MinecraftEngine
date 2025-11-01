#pragma once

#include <Function.hpp>

namespace MCEditor
{

class CameraController : public MCEngine::ScriptableEntity
{
public:
    CameraController(const MCEngine::Entity &entity) : MCEngine::ScriptableEntity(entity) {}
    virtual ~CameraController() override = default;

public:
    void OnStart() override;
    void OnUpdate(float deltaTime) override;

private:
    float m_MoveSpeed = 0.2f;
    float m_RotateSpeed = 2.0f;
    float m_ZoomSpeed = 1.5f;
};

} // namespace MCEditor