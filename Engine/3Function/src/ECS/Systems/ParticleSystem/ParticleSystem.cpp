#include "ParticleSystem.hpp"

Engine::ParticleSystem &Engine::ParticleSystem::GetInstance()
{
    static ParticleSystem instance;
    return instance;
}

void Engine::ParticleSystem::UpdateParticle(ParticleComponent &particleComponent, float deltaTime)
{
    // Update existing particles
    auto &&particles = particleComponent.Particles;
    for (size_t i = 0; i < particles.size();)
    {
        auto &&particle = particles[i];
        if (particle.Lifetime >= particle.LifeLength)
        {
            // Remove particle
            particles[i] = particles.back();
            particles.pop_back();
        }
        else
        {
            // Update particle position
            particle.Position += particle.Velocity * deltaTime;
            i++;
        }
        particle.Lifetime += deltaTime;
    }

    // Emit new particles
    for (int i = 0; i < 10; i++)
    {
        if (particles.size() < particleComponent.GetMaxParticles())
            particles.push_back(particleComponent.EmitFunction());
    }
}

void Engine::ParticleSystem::Render(const ParticleComponent &particleComponent) const
{
    particleComponent.GetVertexArray()->GetVertexBuffer()->SetData(
        particleComponent.Particles.data(), particleComponent.Particles.size() * sizeof(Particle), 0);
    particleComponent.GetVertexArray()->Render(RendererType::Points);
}