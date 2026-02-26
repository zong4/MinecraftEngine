#include "ScenesManager.hpp"

Editor::ScenesManager &Editor::ScenesManager::GetInstance()
{
    static ScenesManager instance;
    return instance;
}

void Editor::ScenesManager::SetActiveScene(const std::shared_ptr<Engine::Scene> &scene)
{
    m_ActiveScene = scene;
    m_ActiveScene->Resize(m_FbWidth, m_FbHeight);
}

void Editor::ScenesManager::PickEntity(int mouseX, int mouseY)
{
    int pickedID = m_ColorIDFrameBuffer->PickPixel(mouseX, mouseY);
    m_SelectedEntity = Engine::Entity((entt::entity)(pickedID - 1), &m_ActiveScene->GetRegistry());
    LOG_ENGINE_INFO("Picked entity ID: " + std::to_string(pickedID) +
                    ", Entity: " + std::to_string((uint32_t)m_SelectedEntity));
}

void Editor::ScenesManager::Resize(int width, int height)
{
    if (m_FbWidth == width && m_FbHeight == height)
        return;
    m_FbWidth = width;
    m_FbHeight = height;

    // Resize both scenes
    m_ColorIDFrameBuffer->Resize(width, height);
    m_EditorScene->Resize(width, height);
    m_ActiveScene->Resize(width, height);
}

void Editor::ScenesManager::UpdateInEditor(float deltaTime)
{
    m_EditorScene->Update(deltaTime);
    m_EditorScene->UpdateRuntime(deltaTime);
    m_ActiveScene->Update(deltaTime);
}

void Editor::ScenesManager::UpdateInRuntime(float deltaTime)
{
    m_ActiveScene->Update(deltaTime);
    m_ActiveScene->UpdateRuntime(deltaTime);
}

void Editor::ScenesManager::RenderInEditor()
{
    m_ActiveScene->Render(m_EditorScene->GetMainCamera(), m_ColorIDFrameBuffer);
}

void Editor::ScenesManager::RenderInRuntime() { m_ActiveScene->Render(m_ActiveScene->GetMainCamera()); }

Editor::ScenesManager::ScenesManager()
{
    m_EditorScene = std::make_shared<Editor::EditorScene>();
    m_ActiveScene = std::make_shared<Editor::ExampleScene>();
}