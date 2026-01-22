#pragma once

#include "../Component/RigidBodyComponent.hpp"
#include "../Component/TransformComponent.hpp"
#include <entt/entt.hpp>

class btDefaultCollisionConfiguration;
class btCollisionDispatcher;
class btBroadphaseInterface;
class btSequentialImpulseConstraintSolver;
class btDiscreteDynamicsWorld;

namespace Engine
{

class PhysicSystem
{
public:
    PhysicSystem();
    ~PhysicSystem();

    // Setters
    void DeleteRigidBody(RigidBodyComponent *rigidBody);

public:
    void Update(entt::registry &registry, float deltaTime);

private:
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