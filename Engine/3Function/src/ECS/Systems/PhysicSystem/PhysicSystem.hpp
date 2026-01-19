#pragma once

#include "../../Components/PhysicComponents.hpp"
#include "../../Components/TransformComponent.hpp"

namespace Engine
{

class PhysicSystem
{
public:
    PhysicSystem();
    ~PhysicSystem();

public:
    void UpdateRigidBody(RigidBodyComponent &rigidBody, const TransformComponent &transform);
    void Update(float deltaTime) { m_DynamicsWorld->stepSimulation(deltaTime); }

private:
    btDefaultCollisionConfiguration *m_CollisionConfiguration;
    btCollisionDispatcher *m_Dispatcher;
    btBroadphaseInterface *m_Broadphase;
    btSequentialImpulseConstraintSolver *m_Solver;
    btDiscreteDynamicsWorld *m_DynamicsWorld;

private:
    void AddCube(RigidBodyComponent &rigidBody, const TransformComponent &transform);
};

} // namespace Engine