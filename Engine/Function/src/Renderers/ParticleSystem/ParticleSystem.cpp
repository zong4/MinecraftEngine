#include "ParticleSystem.hpp"

Engine::ParticleSystem::ParticleSystem(size_t maxParticles) : m_MaxParticles(maxParticles)
{
    m_VertexArray = VertexArray::Create(
        VertexBuffer::Create(sizeof(Particle) * maxParticles),
        std::vector<VertexAttribute>{
            {0, 3, VertexAttributeType::Float, false, sizeof(Particle), (const void *)0},
            {1, 3, VertexAttributeType::Float, false, sizeof(Particle), (const void *)(3 * sizeof(float))},
            {2, 1, VertexAttributeType::Float, false, sizeof(Particle), (const void *)(6 * sizeof(float))},
            {3, 1, VertexAttributeType::Float, false, sizeof(Particle), (const void *)(7 * sizeof(float))}});
}

void Engine::ParticleSystem::AddParticle(const Particle &particle)
{
    if (m_Particles.size() < m_MaxParticles)
        m_Particles.push_back(particle);
}

void Engine::ParticleSystem::Update(float deltaTime)
{
    for (size_t i = 0; i < m_Particles.size();)
    {
        auto &&particle = m_Particles[i];
        if (particle.Lifetime >= particle.LifeLength)
        {
            // Remove particle
            m_Particles[i] = m_Particles.back();
            m_Particles.pop_back();
        }
        else
        {
            // Update particle position
            particle.Position += particle.Velocity * deltaTime;
            i++;
        }
        particle.Lifetime += deltaTime;
    }
}

void Engine::ParticleSystem::Render() const
{
    m_VertexArray->GetVertexBuffer()->SetData(m_Particles.data(), m_Particles.size() * sizeof(Particle), 0);
    m_VertexArray->Render(RendererType::Points);
}