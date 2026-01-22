#pragma once

#include <Platform.hpp>
#include <miniaudio.h>

namespace Engine
{

static ma_engine *GetSoundEngine();

struct SoundComponent
{
public:
    SoundComponent(bool looping = false, float volume = 1.0f) : m_Looping(looping), m_Volume(volume) {}
    ~SoundComponent();

    // Getters
    bool IsLooping() const { return m_Looping; }
    float GetVolume() const { return m_Volume; }
    const ma_sound &GetSound() const { return m_Sound; }

    // Setters
    void SetLooping(bool loop);
    void SetVolume(float volume);
    void SetSound(const std::filesystem::path &filepath);

public:
    void Play();
    void Stop();

private:
    bool m_Looping;
    float m_Volume;
    ma_sound m_Sound{};
};

} // namespace Engine