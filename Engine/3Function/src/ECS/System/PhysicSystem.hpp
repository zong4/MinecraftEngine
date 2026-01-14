#pragma once

#include "../Components/PhysicComponents.hpp"
#include "../Components/TransformComponent.hpp"

namespace Engine
{

class PhysicSystem
{
public:
    PhysicSystem();
    ~PhysicSystem();

public:
    void Update(float deltaTime) { m_DynamicsWorld->stepSimulation(deltaTime); }
    void AddCube(TransformComponent *transform, RigidBodyComponent *rigidBody);

private:
    btDefaultCollisionConfiguration *m_CollisionConfiguration;
    btCollisionDispatcher *m_Dispatcher;
    btBroadphaseInterface *m_Broadphase;
    btSequentialImpulseConstraintSolver *m_Solver;
    btDiscreteDynamicsWorld *m_DynamicsWorld;
};

} // namespace Engine