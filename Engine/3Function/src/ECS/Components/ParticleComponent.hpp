#pragma once

#include <Platform.hpp>

namespace Engine
{

struct Particle
{
    glm::vec3 Position;
    glm::vec3 Velocity;
    float Lifetime;
    float LifeLength;

public:
    Particle(const glm::vec3 &position, const glm::vec3 &velocity, float lifeLength)
        : Position(position), Velocity(velocity), Lifetime(0.0f), LifeLength(lifeLength)
    {
    }
};

struct ParticleComponent
{
    std::function<Particle()> EmitFunction;
    std::vector<Particle> Particles;

public:
    ParticleComponent(std::function<Particle()> emitFunction, int maxParticles = 1000)
        : EmitFunction(emitFunction), m_MaxParticles(maxParticles)
    {
        m_VertexArray = VertexArray::Create(
            VertexBuffer::Create(sizeof(Particle) * maxParticles),
            std::vector<VertexAttribute>{
                {0, 3, VertexAttributeType::Float, false, sizeof(Particle), (const void *)0},
                {1, 3, VertexAttributeType::Float, false, sizeof(Particle), (const void *)(3 * sizeof(float))},
                {2, 1, VertexAttributeType::Float, false, sizeof(Particle), (const void *)(6 * sizeof(float))},
                {3, 1, VertexAttributeType::Float, false, sizeof(Particle), (const void *)(7 * sizeof(float))}});
    }

public:
    // Getters
    int GetMaxParticles() const { return m_MaxParticles; }
    std::shared_ptr<VertexArray> GetVertexArray() const { return m_VertexArray; }

private:
    int m_MaxParticles;
    std::shared_ptr<VertexArray> m_VertexArray;
};

} // namespace Engine