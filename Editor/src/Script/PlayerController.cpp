#include "PlayerController.hpp"

using namespace Engine;

void Editor::PlayerController::OnCreate()
{
    m_Transform = GetComponent<Engine::TransformComponent>();
    m_RigidBody = GetComponent<Engine::RigidBodyComponent>();
}

void Editor::PlayerController::OnUpdate(float deltaTime)
{
    PROFILE_FUNCTION();

    // Move
    glm::vec3 movement = glm::vec3(Input::GetInstance().IsKeyDown(KEY_D) - Input::GetInstance().IsKeyDown(KEY_A), 0.0f,
                                   Input::GetInstance().IsKeyDown(KEY_S) -
                                       Input::GetInstance().IsKeyDown(KEY_W)); // Camera looks along -Z
    m_Transform->Position += movement * m_MoveSpeed * deltaTime;

    // Jump
    if (Input::GetInstance().IsKeyPressed(KEY_J))
    {
        m_Transform->Position.y += m_MoveSpeed * deltaTime;
    }
}
