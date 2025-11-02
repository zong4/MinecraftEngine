#pragma once

#include <Function.hpp>

namespace MCEditor
{

class SceneManager
{
public:
    static SceneManager &GetInstance();

    // Getters
    MCEngine::Entity GetSelectedEntity() const { return m_SelectedEntity; }
    std::shared_ptr<MCEngine::Scene> GetEditorScene() const { return m_EditorScene; }
    std::shared_ptr<MCEngine::Scene> GetActiveScene() const { return m_ActiveScene; }

    // Setters
    void SetSelectedEntity(entt::entity entity);
    void SetSelectedEntity(MCEngine::Entity entity) { m_SelectedEntity = entity; }
    void SetActiveScene(const std::shared_ptr<MCEngine::Scene> &scene);

public:
    void NewExampleScene();
    void OpenScene(const std::string &filepath);
    void OpenSceneDialog();
    void SaveSceneAsDialog() const;

private:
    MCEngine::Entity m_SelectedEntity;
    std::shared_ptr<MCEngine::Scene> m_EditorScene;
    std::shared_ptr<MCEngine::Scene> m_ActiveScene;
    std::vector<std::shared_ptr<MCEngine::Scene>> m_Scenes;

private:
    SceneManager();
    ~SceneManager() = default;

    void NewEmptyScene() { SetActiveScene(std::make_shared<MCEngine::Scene>()); }
    void NewWelcomeScene();
};

} // namespace MCEditor