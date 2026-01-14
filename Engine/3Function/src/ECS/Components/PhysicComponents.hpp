#pragma once

#include "../../Physic/Collider/BoundingBox.hpp"
#include <btBulletDynamicsCommon.h>

namespace Engine
{

struct RigidBodyComponent
{
    bool UseGravity = true;
    float Mass = 1.0f;
    btCollisionShape *Shape = nullptr;
    btRigidBody *Body = nullptr;

    ~RigidBodyComponent()
    {
        if (Body)
        {
            delete Body->getMotionState();
            delete Body;
        }
        if (Shape)
            delete Shape;
    }
};

struct ColliderComponent
{
    BoundingBox BBox;
};

} // namespace Engine