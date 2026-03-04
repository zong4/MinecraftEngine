#include "PhysicSystem.hpp"

Engine::PhysicSystem::PhysicSystem()
{
    m_CollisionConfiguration = new btDefaultCollisionConfiguration();
    m_Dispatcher = new btCollisionDispatcher(m_CollisionConfiguration);
    m_Broadphase = new btDbvtBroadphase();
    m_Solver = new btSequentialImpulseConstraintSolver();
    m_DynamicsWorld = new btDiscreteDynamicsWorld(m_Dispatcher, m_Broadphase, m_Solver, m_CollisionConfiguration);
    m_DynamicsWorld->setGravity(btVector3(0, -98.1f, 0));
}

void Engine::PhysicSystem::Delete(const Entity &entity)
{
    PROFILE_FUNCTION();

    auto &&rigidBody = entity.GetComponent<RigidBodyComponent>();
    if (rigidBody && rigidBody->Body)
    {
        // Clean up motion state
        delete rigidBody->Body->getMotionState();

        // Remove the rigid body from the dynamics world and delete it
        m_DynamicsWorld->removeRigidBody(rigidBody->Body);
        delete rigidBody->Body;
        rigidBody->Body = nullptr;
    }
}

void Engine::PhysicSystem::Update(entt::registry &registry, float deltaTime)
{
    PROFILE_FUNCTION();

    // Fixed timestep update
    m_Accumulator += deltaTime;
    while (m_Accumulator < m_FixedDeltaTime)
        return;
    m_Accumulator -= m_FixedDeltaTime;

    // Sync RigidBodies with Transforms
    auto &&rigibodyView = registry.view<TransformComponent, RigidBodyComponent>();
    for (auto &&entity : rigibodyView)
    {
        auto &&[transform, rigidBody] = rigibodyView.get<TransformComponent, RigidBodyComponent>(entity);
        UpdateRigidBody(rigidBody, transform);
    }

    // Update physics
    m_DynamicsWorld->stepSimulation(deltaTime);

    // Sync Transforms with RigidBodies
    for (auto &&entity : rigibodyView)
    {
        auto &&[transform, rigibody] = rigibodyView.get<TransformComponent, RigidBodyComponent>(entity);

        // Get world transform from rigid body
        btTransform btTransform;
        rigibody.Body->getMotionState()->getWorldTransform(btTransform);

        // Update TransformComponent
        glm::vec3 position(btTransform.getOrigin().getX(), btTransform.getOrigin().getY(),
                           btTransform.getOrigin().getZ());
        transform.Position = position;
        glm::quat rotation(btTransform.getRotation().getW(), btTransform.getRotation().getX(),
                           btTransform.getRotation().getY(), btTransform.getRotation().getZ());
        transform.Rotation = glm::eulerAngles(rotation);
    }
}

void Engine::PhysicSystem::Shutdown()
{
    PROFILE_FUNCTION();

    if (!m_DynamicsWorld)
        return;

    // Clean up all rigid bodies
    for (int i = m_DynamicsWorld->getNumCollisionObjects() - 1; i >= 0; i--)
    {
        btCollisionObject *obj = m_DynamicsWorld->getCollisionObjectArray()[i];
        btRigidBody *body = btRigidBody::upcast(obj);
        if (body)
        {
            // Clean up motion state
            delete body->getMotionState();

            // Remove the rigid body from the dynamics world and delete it
            m_DynamicsWorld->removeRigidBody(body);
            delete body;
            body = nullptr;
        }
    }

    // Clean up dynamics world
    delete m_DynamicsWorld;
    m_DynamicsWorld = nullptr;

    // Clean up solver
    delete m_Solver;
    m_Solver = nullptr;

    // Clean up broadphase
    delete m_Broadphase;
    m_Broadphase = nullptr;

    // Clean up dispatcher
    delete m_Dispatcher;
    m_Dispatcher = nullptr;

    // Clean up collision configuration
    delete m_CollisionConfiguration;
    m_CollisionConfiguration = nullptr;
}

void Engine::PhysicSystem::UpdateRigidBody(RigidBodyComponent &rigidBody, const TransformComponent &transform)
{
    PROFILE_FUNCTION();

    // Create rigid body if it doesn't exist
    if (!rigidBody.Body)
        AddRigidBody(rigidBody, transform);
    btRigidBody *body = rigidBody.Body;

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
        body->setActivationState(DISABLE_DEACTIVATION);
        rigidBody.Body->setAngularFactor(btVector3(0.0f, 1.0f, 0.0f));
        break;
    }
    case RigidBodyType::Kinematic: {
        body->setCollisionFlags((body->getCollisionFlags() & ~btCollisionObject::CF_STATIC_OBJECT) |
                                btCollisionObject::CF_KINEMATIC_OBJECT);
        body->setMassProps(0.0f, btVector3(0, 0, 0));
        // body->setActivationState(DISABLE_DEACTIVATION);
        break;
    }
    }
}

void Engine::PhysicSystem::AddRigidBody(RigidBodyComponent &rigidBody, const TransformComponent &transform)
{
    PROFILE_FUNCTION();

    // Transform
    btTransform btTransform;
    btTransform.setIdentity();
    glm::vec3 position = transform.GetGlobalPosition();
    btTransform.setOrigin(btVector3(position.x, position.y, position.z));
    glm::quat rotationQuat = transform.GetRotationQuat(TransformSpace::Global);
    btTransform.setRotation(btQuaternion(rotationQuat.x, rotationQuat.y, rotationQuat.z, rotationQuat.w));
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

    // Rigid body
    btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, motionState, rigidBody.Shape, inertia);
    rigidBody.Body = new btRigidBody(rbInfo);

    // Body type
    switch (rigidBody.Type)
    {
    case RigidBodyType::Static:
        rigidBody.Body->setCollisionFlags(rigidBody.Body->getCollisionFlags() | btCollisionObject::CF_STATIC_OBJECT);
        break;
    case RigidBodyType::Dynamic:
        rigidBody.Body->setCollisionFlags(rigidBody.Body->getCollisionFlags() &
                                          ~btCollisionObject::CF_KINEMATIC_OBJECT);
        rigidBody.Body->setActivationState(DISABLE_DEACTIVATION);
        rigidBody.Body->setAngularFactor(btVector3(0.0f, 1.0f, 0.0f));
        break;
    case RigidBodyType::Kinematic:
        rigidBody.Body->setCollisionFlags((rigidBody.Body->getCollisionFlags() & ~btCollisionObject::CF_STATIC_OBJECT) |
                                          btCollisionObject::CF_KINEMATIC_OBJECT);
        // rigidBody.Body->setActivationState(DISABLE_DEACTIVATION);
        break;
    }

    // Add to world
    m_DynamicsWorld->addRigidBody(rigidBody.Body);
}