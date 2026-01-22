#include "AudioComponent.hpp"

#define MINIAUDIO_IMPLEMENTATION
#define MINIAUDIO_ENABLE_MP3
#include <miniaudio.h>

ma_engine *Engine::GetAudioEngine()
{
    static ma_engine engine;
    static bool initialized = false;
    if (!initialized)
    {
        ma_engine_init(nullptr, &engine);
        initialized = true;
    }
    return &engine;
}

Engine::AudioComponent::~AudioComponent()
{
    if (m_Initialized)
    {
        ma_sound_uninit(&m_Audio);
        m_Initialized = false;
    }
}

void Engine::AudioComponent::SetLooping(bool loop)
{
    m_Looping = loop;
    if (m_Initialized)
    {
        ma_sound_set_looping(&m_Audio, m_Looping);
    }
}

void Engine::AudioComponent::SetVolume(float volume)
{
    m_Volume = volume;
    if (m_Initialized)
    {
        ma_sound_set_volume(&m_Audio, m_Volume);
    }
}

void Engine::AudioComponent::SetAudio(const std::filesystem::path &filepath)
{
    // Return if audio engine is not initialized
    ma_engine *engine = GetAudioEngine();
    if (engine == nullptr)
    {
        LOG_ENGINE_ERROR("Audio engine is not initialized.");
        return;
    }

    // // Uninitialize previous audio if any
    if (m_Initialized)
    {
        ma_sound_uninit(&m_Audio);
        m_Initialized = false;
    }

    // Initialize sound from file
    if (ma_sound_init_from_file(engine, filepath.string().c_str(), MA_SOUND_FLAG_DECODE, nullptr, nullptr, &m_Audio) !=
        MA_SUCCESS)
    {
        LOG_ENGINE_ERROR("Failed to load Audio: " + filepath.string());
        return;
    }
    m_Initialized = true;
    SetLooping(m_Looping);
    SetVolume(m_Volume);
}

void Engine::AudioComponent::SetPosition(const glm::vec3 &pos)
{
    if (m_Initialized)
    {
        ma_sound_set_position(&m_Audio, pos.x, pos.y, pos.z);
    }
}

void Engine::AudioComponent::Play()
{
    if (m_Initialized)
    {
        ma_sound_start(&m_Audio);
    }
}

void Engine::AudioComponent::Stop()
{
    if (m_Initialized)
    {
        ma_sound_stop(&m_Audio);
    }
}
