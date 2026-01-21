#include "ExampleScene.hpp"

#include "../Script/PlayerController.hpp"
#include "../Script/RotatingLight.hpp"

Editor::ExampleScene::ExampleScene() : Engine::Scene3D()
{
    PROFILE_FUNCTION();

    // ------------------
    // Camera
    // ------------------
    Engine::Entity camera = AddCamera(
        "MainCamera", Engine::TransformComponent(glm::vec3(0.0f, 10.0f, 20.0f), glm::vec3(-30.0f, 0.0f, 0.0f)),
        Engine::CameraComponent(Engine::CameraType::Perspective));
    SetMainCamera(camera);

    // ------------------
    // Light
    // ------------------
    Engine::Entity light = AddLight(
        "DirectionalLight", Engine::TransformComponent(glm::vec3(0.0f, 10.0f, 0.0f), glm::vec3(-45.0f, -45.0f, 0.0f)),
        Engine::LightComponent(Engine::LightType::Directional));
    light.AddComponent<Engine::NativeScriptComponent>();
    light.GetComponent<Engine::NativeScriptComponent>()->Bind<RotatingLight>(light);

    // ------------------
    // Static Plane
    // ------------------
    auto plane = AddCube("Plane", Engine::TransformComponent(glm::vec3(0.0f, -0.5f, 0.0f), glm::vec3(0.0f),
                                                             glm::vec3(15.0f, 1.0f, 15.0f)));
    auto &&planeRb = plane.GetComponent<Engine::RigidBodyComponent>();
    planeRb->Type = Engine::RigidBodyType::Static;

    // ------------------
    // Player
    // ------------------
    auto player = AddCube("Player", Engine::TransformComponent(glm::vec3(0.0f, 1.0f, 0.0f)));
    player.AddComponent<Engine::NativeScriptComponent>();
    player.GetComponent<Engine::NativeScriptComponent>()->Bind<PlayerController>(player);
}
