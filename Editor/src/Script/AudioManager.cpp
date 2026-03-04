#include "AudioManager.hpp"

using namespace Engine;

void Editor::AudioManager::OnCreate()
{
    m_Audio = GetComponent<Engine::AudioComponent>();
    m_Audio->SetAudio(AudiosManager::GetInstance().GetAudio("bgm"));
    m_Audio->SetLooping(true);
    m_Audio->SetVolume(0.5f);
}

void Editor::AudioManager::OnUpdate(float deltaTime)
{
    PROFILE_FUNCTION();

    // Play background music
    if (!m_Audio->IsPlaying())
        m_Audio->Play();

    if (Input::GetInstance().IsKeyPressed(KEY_SPACE))
    {
        if (m_WalkAudio->IsPlaying())
        {
            m_WalkAudio->Stop();
        }
    }
}