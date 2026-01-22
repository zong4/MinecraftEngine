#include "SoundComponent.hpp"

#define MINIAUDIO_IMPLEMENTATION
#define MINIAUDIO_ENABLE_MP3
#include <miniaudio.h>

ma_engine *Engine::GetSoundEngine()
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

Engine::SoundComponent::~SoundComponent() { ma_sound_uninit(&m_Sound); }

void Engine::SoundComponent::SetLooping(bool loop)
{
    m_Looping = loop;
    ma_sound_set_looping(&m_Sound, m_Looping);
}

void Engine::SoundComponent::SetVolume(float volume)
{
    m_Volume = volume;
    ma_sound_set_volume(&m_Sound, m_Volume);
}

void Engine::SoundComponent::SetSound(const std::filesystem::path &filepath)
{
    ma_engine *engine = GetSoundEngine();
    if (engine == nullptr)
    {
        LOG_ENGINE_ERROR("Sound engine is not initialized.");
        return;
    }

    if (ma_sound_init_from_file(engine, filepath.string().c_str(), MA_SOUND_FLAG_DECODE, nullptr, nullptr, &m_Sound) !=
        MA_SUCCESS)
    {
        LOG_ENGINE_ERROR("Failed to load sound: " + filepath.string());
        return;
    }
    SetLooping(m_Looping);
    SetVolume(m_Volume);
}

void Engine::SoundComponent::Play() { ma_sound_start(&m_Sound); }

void Engine::SoundComponent::Stop() { ma_sound_stop(&m_Sound); }
