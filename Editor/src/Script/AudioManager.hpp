#pragma once

#include <Function.hpp>

namespace Editor
{

class AudioManager : public Engine::ScriptableEntity
{
public:
    AudioManager(const Engine::Entity &entity) : Engine::ScriptableEntity(entity) {}
    ~AudioManager() override = default;

public:
    void OnCreate() override;
    void OnUpdate(float deltaTime) override;

private:
    Engine::AudioComponent *m_Audio = nullptr;
};

} // namespace Editor