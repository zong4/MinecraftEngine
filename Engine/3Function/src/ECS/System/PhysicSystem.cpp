#include "PhysicSystem.hpp"

Engine::PhysicSystem::PhysicSystem()
{
    m_CollisionConfiguration = new btDefaultCollisionConfiguration();
    m_Dispatcher = new btCollisionDispatcher(m_CollisionConfiguration);
    m_Broadphase = new btDbvtBroadphase();
    m_Solver = new btSequentialImpulseConstraintSolver();
    m_DynamicsWorld = new btDiscreteDynamicsWorld(m_Dispatcher, m_Broadphase, m_Solver, m_CollisionConfiguration);
    m_DynamicsWorld->setGravity(btVector3(0, -9.81f, 0));
}

Engine::PhysicSystem::~PhysicSystem()
{
    delete m_DynamicsWorld;
    delete m_Solver;
    delete m_Broadphase;
    delete m_Dispatcher;
    delete m_CollisionConfiguration;
}

void Engine::PhysicSystem::AddCube(TransformComponent *transform, RigidBodyComponent *rigidBody)
{
    // Transform
    btTransform btTransform;
    btTransform.setIdentity();
    btTransform.setOrigin(btVector3(transform->Position.x, transform->Position.y, transform->Position.z));
    glm::quat rotationQuat = transform->GetRotationQuat(TransformSpace::Global);
    btTransform.setRotation(btQuaternion(rotationQuat.x, rotationQuat.y, rotationQuat.z, rotationQuat.w));

    // Motion state
    btDefaultMotionState *motionState = new btDefaultMotionState(btTransform);

    // Shape
    btVector3 inertia(0, 0, 0);
    rigidBody->Shape =
        new btBoxShape(btVector3(0.5f * transform->Scale.x, 0.5f * transform->Scale.y, 0.5f * transform->Scale.z));
    if (rigidBody->Mass != 0)
        rigidBody->Shape->calculateLocalInertia(rigidBody->Mass, inertia);

    // Rigid body info
    btRigidBody::btRigidBodyConstructionInfo rbInfo(rigidBody->Mass, motionState, rigidBody->Shape, inertia);
    rigidBody->Body = new btRigidBody(rbInfo);
    rigidBody->Body->setActivationState(DISABLE_DEACTIVATION);

    // Add to world
    m_DynamicsWorld->addRigidBody(rigidBody->Body);
}