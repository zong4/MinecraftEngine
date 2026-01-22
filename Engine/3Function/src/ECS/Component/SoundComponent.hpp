#pragma once

#include <Platform.hpp>

namespace Engine
{

struct SoundComponent
{
    bool Looping;
    float Volume;

public:
    SoundComponent(bool looping = false, float volume = 1.0f) : Looping(looping), Volume(volume) {}

public:
    void Play() {}
};

} // namespace Engine