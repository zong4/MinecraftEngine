#pragma once

#include "../Component/AudioComponent.hpp"
#include "../Component/TransformComponent.hpp"

namespace Engine
{

class AudioSystem
{
public:
    static AudioSystem &GetInstance()
    {
        static AudioSystem instance;
        return instance;
    }

public:
    void UpdateAudio(AudioComponent &audioComponent, const TransformComponent &transformComponent)
    {
        audioComponent.SetPosition(transformComponent.Position);
    }

private:
    AudioSystem() = default;
    ~AudioSystem() = default;
};

} // namespace Engine