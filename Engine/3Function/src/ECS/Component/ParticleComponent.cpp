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

void Engine::ParticleComponent::Update(float deltaTime, TransformComponent *transform)
{
    if (!Active)
        return;

    // Update existing particles
    for (auto &&particle : m_Particles)
    {
        particle.Position += particle.Velocity * deltaTime;
        particle.Lifetime += deltaTime;
    }

    // Remove dead particles
    m_Particles.erase(std::remove_if(m_Particles.begin(), m_Particles.end(),
                                     [](const Particle &p) { return p.Lifetime >= p.LifeLength; }),
                      m_Particles.end());

    // If transform is provided, emit particles at the transform's position
    if (transform)
    {
        for (int i = 0; i < m_EmissionCount; i++)
        {
            if (m_Particles.size() >= m_MaxCount)
                break;
            Particle newParticle = EmitFunction();
            newParticle.Position += transform->Position; // Emit at the entity's position
            m_Particles.push_back(newParticle);
        }
    }
    else
    {
        for (int i = 0; i < m_EmissionCount; i++)
        {
            if (m_Particles.size() >= m_MaxCount)
                break;
            m_Particles.push_back(EmitFunction());
        }
    }
}

void Engine::ParticleComponent::Render() const
{
    if (m_Particles.empty() || !Active)
        return;

    m_VertexArray->GetVertexBuffer()->SetData(m_Particles.data(), m_Particles.size() * sizeof(Particle), 0);
    m_VertexArray->Render(RendererType::Points);
}