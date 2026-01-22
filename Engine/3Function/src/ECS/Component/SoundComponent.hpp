#pragma once

#include <Platform.hpp>

class ma_engine;
class ma_sound;

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
    ma_sound *GetSound() const { return m_Sound; }

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
    ma_sound *m_Sound = nullptr;
};

} // namespace Engine