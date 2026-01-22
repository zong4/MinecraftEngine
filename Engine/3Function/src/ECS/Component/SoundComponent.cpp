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

Engine::SoundComponent::~SoundComponent()
{
    if (m_Sound)
    {
        ma_sound_uninit(m_Sound);
        delete m_Sound;
        m_Sound = nullptr;
    }
}

void Engine::SoundComponent::SetLooping(bool loop)
{
    m_Looping = loop;
    if (m_Sound)
        ma_sound_set_looping(m_Sound, m_Looping);
}

void Engine::SoundComponent::SetVolume(float volume)
{
    m_Volume = volume;
    if (m_Sound)
        ma_sound_set_volume(m_Sound, m_Volume);
}

void Engine::SoundComponent::SetSound(const std::filesystem::path &filepath)
{
    ma_engine *engine = GetSoundEngine();
    if (ma_sound_init_from_file(engine, filepath.string().c_str(), MA_SOUND_FLAG_DECODE, nullptr, nullptr, m_Sound) !=
        MA_SUCCESS)
    {
        LOG_ENGINE_ERROR("Failed to load sound: " + filepath.string());
        m_Sound = nullptr;
        return;
    }
    SetLooping(m_Looping);
    SetVolume(m_Volume);
}

void Engine::SoundComponent::Play()
{
    if (m_Sound)
        ma_sound_start(m_Sound);
}

void Engine::SoundComponent::Stop()
{
    if (m_Sound)
        ma_sound_stop(m_Sound);
}
