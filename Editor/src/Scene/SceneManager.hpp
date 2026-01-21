#pragma once

#include "EditorScene.hpp"
#include "ExampleScene.hpp"
#include "WelcomeScene.hpp"

namespace Editor
{

class SceneManager
{
public:
    static SceneManager &GetInstance()
    {
        static SceneManager instance;
        return instance;
    }

    // Getters
    std::shared_ptr<Engine::Scene> GetEditorScene() { return m_EditorScene; }
    std::shared_ptr<Engine::Scene> GetActiveScene() { return m_ActiveScene; }

private:
    std::shared_ptr<Engine::Scene> m_EditorScene = std::make_shared<Editor::EditorScene>();
    std::shared_ptr<Engine::Scene> m_ActiveScene = std::make_shared<Editor::ExampleScene>();

private:
    SceneManager() = default;
    ~SceneManager() = default;
};

} // namespace Editor