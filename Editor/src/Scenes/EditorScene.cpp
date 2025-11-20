#include "EditorScene.hpp"

#include "Scripts/CameraController.hpp"

Editor::EditorScene::EditorScene() : Engine::Scene()
{
    PROFILE_FUNCTION();

    Engine::Entity camera =
        AddCamera("MainCamera", Engine::TransformComponent(), Engine::CameraComponent(Engine::CameraType::Perspective));
    camera.AddComponent<Engine::NativeScriptComponent>();
    camera.GetComponent<Engine::NativeScriptComponent>()->Bind<CameraController>(camera);
    SetMainCamera(camera);
}