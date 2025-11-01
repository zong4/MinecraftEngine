#include "EditorScene.hpp"

#include "Script/CameraController.hpp"

MCEditor::EditorScene::EditorScene() : MCEngine::Scene()
{
    ENGINE_PROFILE_FUNCTION();

    m_MainCamera = AddCamera("MainCamera", MCEngine::TransformComponent(),
                             MCEngine::CameraComponent(MCEngine::CameraType::Perspective));
    m_MainCamera.AddComponent<MCEngine::NativeScriptComponent>().Bind<CameraController>(m_MainCamera);
}