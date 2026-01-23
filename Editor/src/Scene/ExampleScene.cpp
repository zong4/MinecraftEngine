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
    // 2D Sprite
    // ------------------
    AddSquare("ReferenceSprite",
              Engine::TransformComponent(glm::vec3(0.0f, 5.0f, 0.0f), glm::vec3(0.0f), glm::vec3(3.0f)));

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
    plane.AddComponent<Engine::RigidBodyComponent>();
    auto &&planeRb = plane.GetComponent<Engine::RigidBodyComponent>();
    planeRb->Type = Engine::RigidBodyType::Static;

    // ------------------
    // Player
    // ------------------
    auto player = AddCube("Player", Engine::TransformComponent(glm::vec3(0.0f, 3.0f, 0.0f)));
    player.AddComponent<Engine::AudioComponent>(true, 1.0f);
    player.AddComponent<Engine::RigidBodyComponent>();
    player.AddComponent<Engine::NativeScriptComponent>();
    player.GetComponent<Engine::NativeScriptComponent>()->Bind<PlayerController>(player);

    // ------------------
    // Dynamic Cubes Random Generation
    // ------------------
    const int numCubes = 10;
    for (int i = 0; i < numCubes; i++)
    {
        // Position
        glm::vec3 randomPosition = glm::vec3(
            Engine::Random::GetInstance().NextFloat(plane.GetComponent<Engine::TransformComponent>()->Scale.x * -0.5f,
                                                    plane.GetComponent<Engine::TransformComponent>()->Scale.x * 0.5f),
            5.0f + i * 2.0f, // Y: Starting from 5, each cube stacked
            Engine::Random::GetInstance().NextFloat(plane.GetComponent<Engine::TransformComponent>()->Scale.z * -0.5f,
                                                    plane.GetComponent<Engine::TransformComponent>()->Scale.z * 0.5f));

        // Random rotation (Euler angles, converted from 0~360 degrees to radians)
        glm::vec3 randomEuler = glm::vec3(Engine::Random::GetInstance().NextFloat(0.0f, 360.0f),
                                          Engine::Random::GetInstance().NextFloat(0.0f, 360.0f),
                                          Engine::Random::GetInstance().NextFloat(0.0f, 360.0f));

        // Random scale (0.5 ~ 2.0)
        glm::vec3 randomScale = glm::vec3(Engine::Random::GetInstance().NextFloat(0.5f, 2.0f),
                                          Engine::Random::GetInstance().NextFloat(0.5f, 2.0f),
                                          Engine::Random::GetInstance().NextFloat(0.5f, 2.0f));

        // Create cube
        auto cube =
            AddCube("Cube" + std::to_string(i), Engine::TransformComponent(randomPosition, randomEuler, randomScale));
        cube.AddComponent<Engine::RigidBodyComponent>();
    }
}
