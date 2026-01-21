#pragma once

#include "../../Physic/Collider/BoundingBox.hpp"
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
    btCollisionShape *Shape;
    btRigidBody *Body;

public:
    RigidBodyComponent(RigidBodyType type = RigidBodyType::Dynamic, float mass = 1.0f)
        : Type(type), Mass(mass), Shape(nullptr), Body(nullptr)
    {
    }
    ~RigidBodyComponent()
    {
        if (Shape)
            delete Shape;
    }

    // Copy and move semantics
    // RigidBodyComponent(const RigidBodyComponent &) = delete;
    // RigidBodyComponent &operator=(const RigidBodyComponent &) = delete;
};

struct ColliderComponent
{
    BoundingBox BBox;
};

} // namespace Engine