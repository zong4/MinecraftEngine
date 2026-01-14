#pragma once

#include "../../Physics/BoundingBox.hpp"
#include <btBulletDynamicsCommon.h>

namespace Engine
{

struct RigidBodyComponent
{
    bool useGravity = true;
    float mass = 1.0f;
    btRigidBody *body = nullptr;
    btCollisionShape *shape = nullptr;

    ~RigidBodyComponent()
    {
        if (body)
            delete body;
        if (shape)
            delete shape;
    }
};

} // namespace Engine