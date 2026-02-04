#include "EditorScene.hpp"

#include "../Script/CameraController.hpp"

Editor::EditorScene::EditorScene() : Engine::Scene3D("EditorScene")
{
    PROFILE_FUNCTION();

    // Camera
    auto camera =
        AddCamera("MainCamera", Engine::TransformComponent(), Engine::CameraComponent(Engine::CameraType::Perspective));
    camera.AddComponent<Engine::NativeScriptComponent>();
    camera.GetComponent<Engine::NativeScriptComponent>()->Bind<CameraController>(camera);
    SetMainCamera(camera);
}