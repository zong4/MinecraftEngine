#pragma once

#include "../Component/PhysicComponent.hpp"
#include "../Component/TransformComponent.hpp"
#include "../Entity/Entity.hpp"

namespace Engine
{

class PhysicSystem
{
public:
    PhysicSystem();
    ~PhysicSystem() = default;

    // Setters
    void Delete(const Entity &entity);

public:
    void Update(entt::registry &registry, float deltaTime);
    void Shutdown();

private:
    float m_FixedDeltaTime = 1.0f / 60.0f;
    float m_Accumulator = 0.0f;
    btDefaultCollisionConfiguration *m_CollisionConfiguration;
    btCollisionDispatcher *m_Dispatcher;
    btBroadphaseInterface *m_Broadphase;
    btSequentialImpulseConstraintSolver *m_Solver;
    btDiscreteDynamicsWorld *m_DynamicsWorld;

private:
    void UpdateRigidBody(RigidBodyComponent &rigidBody, const TransformComponent &transform);
    void AddRigidBody(RigidBodyComponent &rigidBody, const TransformComponent &transform); // Only Cube for now
};

} // namespace Engine