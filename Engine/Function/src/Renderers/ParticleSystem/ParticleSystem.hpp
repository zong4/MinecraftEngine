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

class ParticleSystem
{
public:
    ParticleSystem(size_t maxParticles);
    ~ParticleSystem() = default;

    void AddParticle(const Particle &particle);

public:
    void Update(float deltaTime);
    void Render() const;

private:
    size_t m_MaxParticles;
    std::vector<Particle> m_Particles;
    std::shared_ptr<VertexArray> m_VertexArray;
};

} // namespace Engine