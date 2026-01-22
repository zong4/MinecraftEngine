#pragma once

#include <Platform.hpp>
#include <miniaudio.h>

namespace Engine
{

static ma_engine *GetAudioEngine();

struct AudioComponent
{
public:
    AudioComponent(bool looping = false, float volume = 1.0f) : m_Looping(looping), m_Volume(volume) {}
    ~AudioComponent();

    // Getters
    bool IsInitialized() const { return m_Initialized; }
    bool IsLooping() const { return m_Looping; }
    float GetVolume() const { return m_Volume; }

    // Setters
    void SetLooping(bool loop);
    void SetVolume(float volume);
    void SetAudio(const std::filesystem::path &filepath);
    void SetPosition(const glm::vec3 &pos);

public:
    void Play();
    void Stop();

private:
    bool m_Initialized = false;
    bool m_Looping;
    float m_Volume;
    ma_sound m_Audio{};
};

} // namespace Engine