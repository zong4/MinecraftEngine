#pragma once

#include "../../Components/ParticleComponent.hpp"

namespace Engine
{

class ParticleSystem
{
public:
    static ParticleSystem &GetInstance();

public:
    void UpdateParticle(ParticleComponent &particleComponent, float deltaTime);
    void Render(const ParticleComponent &particleComponent) const;

private:
    ParticleSystem() = default;
    ~ParticleSystem() = default;
};

} // namespace Engine