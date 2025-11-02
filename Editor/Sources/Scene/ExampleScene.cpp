#include "ExampleScene.hpp"

MCEditor::ExampleScene::ExampleScene() : MCEngine::Scene()
{
    SetMainCamera(AddCamera("MainCamera",
                            MCEngine::TransformComponent(glm::vec3(0.0f, 5.0f, 8.0f), glm::vec3(-30.0f, 0.0f, 0.0f)),
                            MCEngine::CameraComponent(MCEngine::CameraType::Perspective)));

    AddLight("DirectionalLight",
             MCEngine::TransformComponent(glm::vec3(0.0f, 10.0f, 0.0f), glm::vec3(-45.0f, -45.0f, 0.0f)),
             MCEngine::LightComponent(MCEngine::LightType::Directional));

    AddSkybox("Skybox", MCEngine::SkyboxComponent());

    Add3DObject("Cube", MCEngine::TransformComponent(glm::vec3(-2.0f, 0.5f, 0.0f), glm::vec3(0.0f), glm::vec3(1.0f)),
                MCEngine::MeshRendererComponent(
                    MCEngine::Material(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), 0.1f, 0.8f, 0.5f, 32.0f)));
}