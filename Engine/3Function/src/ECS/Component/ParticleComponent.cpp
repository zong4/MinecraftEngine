#include "ParticleComponent.hpp"

Engine::ParticleComponent::ParticleComponent(std::function<Particle()> emitFunction, int maxParticles)
    : EmitFunction(emitFunction), m_MaxParticles(maxParticles)
{
    m_VertexArray = VertexArray::Create(
        VertexBuffer::Create(sizeof(Particle) * maxParticles),
        std::vector<VertexAttribute>{
            {0, 3, VertexAttributeType::Float, false, sizeof(Particle), (const void *)0},                   // Position
            {1, 3, VertexAttributeType::Float, false, sizeof(Particle), (const void *)(3 * sizeof(float))}, // Velocity
            {2, 1, VertexAttributeType::Float, false, sizeof(Particle), (const void *)(6 * sizeof(float))}, // Lifetime
            {3, 1, VertexAttributeType::Float, false, sizeof(Particle), (const void *)(7 * sizeof(float))} // LifeLength
        });
}