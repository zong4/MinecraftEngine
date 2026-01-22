#include "ParticleComponent.hpp"

Engine::ParticleComponent::ParticleComponent(std::function<Particle()> emitFunction, int emissionCount, int maxCount)
    : EmitFunction(emitFunction), m_EmissionCount(emissionCount), m_MaxCount(maxCount)
{
    m_VertexArray = VertexArray::Create(
        VertexBuffer::Create(sizeof(Particle) * maxCount),
        std::vector<VertexAttribute>{
            {0, 3, VertexAttributeType::Float, false, sizeof(Particle), (const void *)0},                   // Position
            {1, 3, VertexAttributeType::Float, false, sizeof(Particle), (const void *)(3 * sizeof(float))}, // Velocity
            {2, 1, VertexAttributeType::Float, false, sizeof(Particle), (const void *)(6 * sizeof(float))}, // Lifetime
            {3, 1, VertexAttributeType::Float, false, sizeof(Particle), (const void *)(7 * sizeof(float))} // LifeLength
        });
}

void Engine::ParticleComponent::Update(float deltaTime)
{
    // Update existing particles
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

    // Emit new particles
    for (int i = 0; i < m_EmissionCount; i++)
    {
        if (static_cast<int>(m_Particles.size()) >= m_MaxCount)
            break;
        m_Particles.push_back(EmitFunction());
    }
}

void Engine::ParticleComponent::Render() const
{
    m_VertexArray->GetVertexBuffer()->SetData(m_Particles.data(), m_Particles.size() * sizeof(Particle), 0);
    m_VertexArray->Render(RendererType::Points);
}