#pragma once

#include <Platform.hpp>
#include <btBulletDynamicsCommon.h>

namespace Engine
{

enum class RigidBodyType
{
    Static = 0,
    Dynamic,
    Kinematic
};

struct RigidBodyComponent
{
    RigidBodyType Type;
    float Mass;
    btCollisionShape *Shape = nullptr;
    btRigidBody *Body = nullptr;

public:
    RigidBodyComponent(RigidBodyType type = RigidBodyType::Dynamic, float mass = 1.0f) : Type(type), Mass(mass) {}
    ~RigidBodyComponent()
    {
        if (Shape)
        {
            delete Shape;
            Shape = nullptr;
        }
    }

public:
    void Reset()
    {
        if (Body)
        {
            Body->setLinearVelocity(btVector3(0, 0, 0));
            Body->setAngularVelocity(btVector3(0, 0, 0));
            Body->clearForces();
            Body->setWorldTransform(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0.0f, 1.0f, 9.0f)));
        }
    }

    // Copy and move semantics
    // RigidBodyComponent(const RigidBodyComponent &) = delete;
    // RigidBodyComponent &operator=(const RigidBodyComponent &) = delete;
};

} // namespace Engine