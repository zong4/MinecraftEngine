#include "PhysicSystem.hpp"

#include <btBulletDynamicsCommon.h>

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
    if (!m_DynamicsWorld)
        return;

    for (int i = m_DynamicsWorld->getNumCollisionObjects() - 1; i >= 0; --i)
    {
        btCollisionObject *obj = m_DynamicsWorld->getCollisionObjectArray()[i];
        btRigidBody *body = btRigidBody::upcast(obj);
        if (body)
        {
            delete body->getMotionState();
            m_DynamicsWorld->removeRigidBody(body);
            delete body;
        }
    }

    delete m_DynamicsWorld;
    m_DynamicsWorld = nullptr;
    delete m_Solver;
    delete m_Broadphase;
    delete m_Dispatcher;
    delete m_CollisionConfiguration;
}

void Engine::PhysicSystem::UpdateRigidBody(RigidBodyComponent &rigidBody, const TransformComponent &transform)
{
    if (!rigidBody.Body)
        AddCube(rigidBody, transform);
    btRigidBody *body = rigidBody.Body;

    // Transform
    btTransform btTransform;
    btTransform.setIdentity();
    btTransform.setOrigin(btVector3(transform.Position.x, transform.Position.y, transform.Position.z));
    glm::quat rotationQuat = transform.GetRotationQuat(TransformSpace::Global);
    btTransform.setRotation(btQuaternion(rotationQuat.x, rotationQuat.y, rotationQuat.z, rotationQuat.w));

    // Update transform from TransformComponent
    body->getMotionState()->setWorldTransform(btTransform);
    body->setWorldTransform(btTransform);

    // Update RigidBodyType
    switch (rigidBody.Type)
    {
    case RigidBodyType::Static: {
        body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_STATIC_OBJECT);
        body->setMassProps(0.0f, btVector3(0, 0, 0));
        break;
    }
    case RigidBodyType::Dynamic: {
        body->setCollisionFlags(body->getCollisionFlags() & ~btCollisionObject::CF_KINEMATIC_OBJECT);
        rigidBody.Mass = fmax(rigidBody.Mass, 0.01f); // Prevent zero mass for dynamic bodies
        btVector3 inertia(0, 0, 0);
        rigidBody.Shape->calculateLocalInertia(rigidBody.Mass, inertia);
        body->setMassProps(rigidBody.Mass, inertia);
        break;
    }
    case RigidBodyType::Kinematic: {
        body->setCollisionFlags((body->getCollisionFlags() & ~btCollisionObject::CF_STATIC_OBJECT) |
                                btCollisionObject::CF_KINEMATIC_OBJECT);
        body->setMassProps(0.0f, btVector3(0, 0, 0));
        body->setActivationState(DISABLE_DEACTIVATION);
        break;
    }
    }
}

void Engine::PhysicSystem::Update(float deltaTime) { m_DynamicsWorld->stepSimulation(deltaTime); }

void Engine::PhysicSystem::AddCube(RigidBodyComponent &rigidBody, const TransformComponent &transform)
{
    // Transform
    btTransform btTransform;
    btTransform.setIdentity();
    btTransform.setOrigin(btVector3(transform.Position.x, transform.Position.y, transform.Position.z));
    glm::quat rotationQuat = transform.GetRotationQuat(TransformSpace::Global);
    btTransform.setRotation(btQuaternion(rotationQuat.x, rotationQuat.y, rotationQuat.z, rotationQuat.w));

    // Motion state
    btDefaultMotionState *motionState = new btDefaultMotionState(btTransform);

    // Shape
    rigidBody.Shape =
        new btBoxShape(btVector3(transform.Scale.x * 0.5f, transform.Scale.y * 0.5f, transform.Scale.z * 0.5f));

    // Mass & inertia
    btScalar mass = 0.0f;
    btVector3 inertia(0, 0, 0);
    if (rigidBody.Type == RigidBodyType::Dynamic)
    {
        rigidBody.Mass = fmax(rigidBody.Mass, 0.1f);
        mass = rigidBody.Mass;
        rigidBody.Shape->calculateLocalInertia(mass, inertia);
    }

    // Rigid body info
    btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, motionState, rigidBody.Shape, inertia);
    rigidBody.Body = new btRigidBody(rbInfo);

    // Body type
    switch (rigidBody.Type)
    {
    case RigidBodyType::Static:
        rigidBody.Body->setCollisionFlags(rigidBody.Body->getCollisionFlags() | btCollisionObject::CF_STATIC_OBJECT);
        break;
    case RigidBodyType::Dynamic:
        rigidBody.Body->setActivationState(DISABLE_DEACTIVATION);
        break;
    case RigidBodyType::Kinematic:
        rigidBody.Body->setCollisionFlags(rigidBody.Body->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
        rigidBody.Body->setActivationState(DISABLE_DEACTIVATION);
        break;
    }

    // Add to world
    m_DynamicsWorld->addRigidBody(rigidBody.Body);
}