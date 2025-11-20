#include "EditorScene.hpp"

#include "Scripts/CameraController.hpp"

Editor::EditorScene::EditorScene() : MCEngine::Scene()
{
    ENGINE_PROFILE_FUNCTION();

    MCEngine::Entity camera = AddCamera("MainCamera", MCEngine::TransformComponent(),
                                        MCEngine::CameraComponent(MCEngine::CameraType::Perspective));
    camera.AddComponent<MCEngine::NativeScriptComponent>();
    camera.GetComponent<MCEngine::NativeScriptComponent>()->Bind<CameraController>(camera);
    SetMainCamera(camera);
}