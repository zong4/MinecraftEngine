#include "ExampleScene.hpp"

Editor::ExampleScene::ExampleScene() : Engine::Scene()
{
    
    Engine::Entity camera =  AddCamera("MainCamera",
                            Engine::TransformComponent(glm::vec3(0.0f, 5.0f, 8.0f), glm::vec3(-30.0f, 0.0f, 0.0f)),
                            Engine::CameraComponent(Engine::CameraType::Perspective));   
    
    
    SetMainCamera(camera);    

    Engine::Entity light = AddLight("DirectionalLight",
                                     Engine::TransformComponent(glm::vec3(0.0f, 100.0f, 0.0f), glm::vec3(-45.0f, -45.0f, 0.0f)),
                                     Engine::LightComponent(Engine::LightType::Directional));

    AddSkybox("Skybox", Engine::SkyboxComponent());

    // Add 3D object with new MaterialComponent and MeshRendererComponent
    auto cube =
        Add3DObject("Cube", Engine::TransformComponent(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f), glm::vec3(1.0f)),
                    Engine::MaterialComponent());
}