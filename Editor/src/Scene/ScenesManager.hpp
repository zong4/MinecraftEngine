#pragma once

#include "EditorScene.hpp"
#include "ExampleScene.hpp"
#include "WelcomeScene.hpp"
#include <Platform.hpp>

namespace Editor
{

class ScenesManager
{
public:
    static ScenesManager &GetInstance();

    // Getters
    Engine::Entity GetSelectedEntity() { return m_SelectedEntity; }
    std::shared_ptr<Engine::Scene> GetEditorScene() { return m_EditorScene; }
    std::shared_ptr<Engine::Scene> GetActiveScene() { return m_ActiveScene; }

    // Setters
    void SetSelectedEntity(const Engine::Entity &entity) { m_SelectedEntity = entity; }
    void SetActiveScene(const std::shared_ptr<Engine::Scene> &scene);

public:
    void PickEntity(int mouseX, int mouseY);
    void Resize(int width, int height);
    void UpdateInEditor(float deltaTime);
    void UpdateInRuntime(float deltaTime);
    void RenderInEditor();
    void RenderInRuntime();

private:
    int m_FbWidth, m_FbHeight;
    Engine::Entity m_SelectedEntity;
    std::shared_ptr<Engine::Scene> m_EditorScene;
    std::shared_ptr<Engine::Scene> m_ActiveScene;
    std::shared_ptr<Engine::FrameBuffer> m_ColorIDFrameBuffer =
        Engine::FrameBuffer::Create(Engine::Texture2DType::Integer, 1280, 720);

private:
    ScenesManager();
    ~ScenesManager() = default;
};

} // namespace Editor