#include "PlayerController.hpp"

#include "../Scene/ScenesManager.hpp"

using namespace Engine;

void Editor::PlayerController::OnCreate()
{
    m_Transform = GetComponent<Engine::TransformComponent>();
    m_RigidBody = GetComponent<Engine::RigidBodyComponent>();
    m_Audio = GetComponent<Engine::AudioComponent>();
    m_Audio->SetAudio(AudiosManager::GetInstance().GetAudio("walk"));
}

void Editor::PlayerController::OnUpdate(float deltaTime)
{
    PROFILE_FUNCTION();

    // Move
    glm::vec3 movement = glm::vec3(Input::GetInstance().IsKeyDown(KEY_D) - Input::GetInstance().IsKeyDown(KEY_A), 0.0f,
                                   Input::GetInstance().IsKeyDown(KEY_S) -
                                       Input::GetInstance().IsKeyDown(KEY_W)); // Camera looks along -Z
    m_RigidBody->Body->setLinearVelocity(
        btVector3(movement.x * m_MoveSpeed, m_RigidBody->Body->getLinearVelocity().getY(), movement.z * m_MoveSpeed));

    // Jump
    if (Input::GetInstance().IsKeyPressed(KEY_K))
        m_RigidBody->Body->applyCentralImpulse(btVector3(0, m_JumpStrength, 0));

    // Play Audio when moving
    if (glm::length(movement) > 0.0f)
        m_Audio->Play();
    else
        m_Audio->Stop();

    // Camera follow
    auto &&cameraTransform =
        ScenesManager::GetInstance().GetActiveScene()->GetMainCamera().GetComponent<Engine::TransformComponent>();
    cameraTransform->Position = m_Transform->Position + m_Offset;
    cameraTransform->SetRotationEuler(m_CameraRotation);
}
