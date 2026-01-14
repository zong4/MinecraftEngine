#include "ExampleScene.hpp"

#include "Scripts/RotatingLight.hpp"

Editor::ExampleScene::ExampleScene() : Engine::Scene3D()
{
    Engine::Entity camera =
        AddCamera("MainCamera", Engine::TransformComponent(glm::vec3(0.0f, 5.0f, 8.0f), glm::vec3(-30.0f, 0.0f, 0.0f)),
                  Engine::CameraComponent(Engine::CameraType::Perspective));
    SetMainCamera(camera);

    Engine::Entity light = AddLight(
        "DirectionalLight", Engine::TransformComponent(glm::vec3(0.0f, 10.0f, 0.0f), glm::vec3(-45.0f, -45.0f, 0.0f)),
        Engine::LightComponent(Engine::LightType::Directional));
    light.AddComponent<Engine::NativeScriptComponent>();
    light.GetComponent<Engine::NativeScriptComponent>()->Bind<RotatingLight>(light);

    auto cube =
        Add3DObject("Cube", Engine::TransformComponent(glm::vec3(-2.0f, 0.5f, 0.0f), glm::vec3(0.0f), glm::vec3(1.0f)));
}