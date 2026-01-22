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

public:
    ParticleComponent(std::function<Particle()> emitFunction, int emissionCount = 10, int maxCount = 1000);

public:
    void Update(float deltaTime);
    void Render() const;

private:
    int m_EmissionCount;
    int m_MaxCount;
    std::vector<Particle> m_Particles;
    std::shared_ptr<VertexArray> m_VertexArray;
};

} // namespace Engine