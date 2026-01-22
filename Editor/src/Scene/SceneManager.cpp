#include "SceneManager.hpp"

#include "EditorScene.hpp"
#include "ExampleScene.hpp"
#include "WelcomeScene.hpp"

Editor::SceneManager &Editor::SceneManager::GetInstance()
{
    static SceneManager instance;
    return instance;
}

void Editor::SceneManager::SetActiveScene(const std::shared_ptr<Engine::Scene> &scene)
{
    m_ActiveScene = scene;
    m_ActiveScene->Resize(m_FbWidth, m_FbHeight);
}

void Editor::SceneManager::Resize(int width, int height)
{
    if (m_FbWidth == width && m_FbHeight == height)
        return;
    m_FbWidth = width;
    m_FbHeight = height;

    // Resize both scenes
    m_EditorScene->Resize(width, height);
    m_ActiveScene->Resize(width, height);
}

void Editor::SceneManager::UpdateInEditor(float deltaTime)
{
    m_EditorScene->Update(deltaTime);
    m_EditorScene->UpdateRuntime(deltaTime);
    m_ActiveScene->Update(deltaTime);
}

void Editor::SceneManager::UpdateInRuntime(float deltaTime)
{
    m_ActiveScene->Update(deltaTime);
    m_ActiveScene->UpdateRuntime(deltaTime);
}

void Editor::SceneManager::RenderInEditor() { m_ActiveScene->Render(m_EditorScene->GetMainCamera()); }

void Editor::SceneManager::RenderInRuntime() { m_ActiveScene->Render(m_ActiveScene->GetMainCamera()); }

Editor::SceneManager::SceneManager()
{
    m_EditorScene = std::make_shared<Editor::EditorScene>();
    m_ActiveScene = std::make_shared<Editor::ExampleScene>();
}
