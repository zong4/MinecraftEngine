#include "SceneManager.hpp"

#include "Manager/ConfigManager.hpp"
#include "Scene/EditorScene.hpp"
#include "Scene/ExampleScene.hpp"
#include "Scene/WelcomeScene.hpp"

MCEditor::SceneManager &MCEditor::SceneManager::GetInstance()
{
    static SceneManager instance;
    return instance;
}

void MCEditor::SceneManager::SetSelectedEntity(entt::entity entity)
{
    m_SelectedEntity = MCEngine::Entity(entity, &m_ActiveScene->GetRegistry());
}

void MCEditor::SceneManager::SetActiveScene(const std::shared_ptr<MCEngine::Scene> &scene)
{
    m_ActiveScene = scene;
    // if (m_ActiveScene)
    //     m_SelectedEntity = MCEngine::Entity((entt::entity)0, &m_ActiveScene->GetRegistry());
}

void MCEditor::SceneManager::NewExampleScene() { SetActiveScene(std::make_shared<MCEditor::ExampleScene>()); }

void MCEditor::SceneManager::OpenScene(const std::string &filepath)
{
    NewEmptyScene();
    MCEngine::SceneSerializer::Deserialize(m_ActiveScene, filepath);
    SetActiveScene(m_ActiveScene);
}

void MCEditor::SceneManager::OpenSceneDialog()
{
    const char *filters[] = {"*.mcscene"};
    const char *file = tinyfd_openFileDialog(
        "Open Scene", ConfigManager::GetInstance().GetScenesPath().string().c_str(), 1, filters, nullptr, 0);
    if (file)
    {
        NewEmptyScene();
        MCEngine::SceneSerializer::Deserialize(m_ActiveScene, file);
        SetActiveScene(m_ActiveScene);
    }
}

void MCEditor::SceneManager::SaveSceneAsDialog() const
{
    const char *filters[] = {"*.mcscene"};
    const char *filepath = tinyfd_saveFileDialog(
        "Save Scene As",
        (ConfigManager::GetInstance().GetScenesPath() / (m_ActiveScene->GetName() + ".mcscene")).string().c_str(), 1,
        filters, nullptr);

    if (filepath)
    {
        std::string file = filepath;
        if (std::filesystem::path(file).extension() != ".mcscene")
            file += ".mcscene";
        MCEngine::SceneSerializer::Serialize(m_ActiveScene, file);
    }
}

MCEditor::SceneManager::SceneManager()
{
    m_EditorScene = std::make_shared<MCEditor::EditorScene>();

    NewWelcomeScene();
    m_Scenes.push_back(m_ActiveScene);
}

void MCEditor::SceneManager::NewWelcomeScene() { SetActiveScene(std::make_shared<MCEditor::WelcomeScene>()); }