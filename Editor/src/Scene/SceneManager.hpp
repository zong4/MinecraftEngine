#pragma once

#include <Platform.hpp>

namespace Editor
{

class SceneManager
{
public:
    static SceneManager &GetInstance();

    // Getters
    std::shared_ptr<Engine::Scene> GetEditorScene() { return m_EditorScene; }
    std::shared_ptr<Engine::Scene> GetActiveScene() { return m_ActiveScene; }

    // Setters
    void SetActiveScene(const std::shared_ptr<Engine::Scene> &scene);

public:
    void Resize(int width, int height);
    void UpdateInEditor(float deltaTime);
    void UpdateInRuntime(float deltaTime);
    void RenderInEditor();
    void RenderInRuntime();

private:
    int m_FbWidth, m_FbHeight;
    std::shared_ptr<Engine::Scene> m_EditorScene;
    std::shared_ptr<Engine::Scene> m_ActiveScene;

private:
    SceneManager();
    ~SceneManager() = default;
};

} // namespace Editor