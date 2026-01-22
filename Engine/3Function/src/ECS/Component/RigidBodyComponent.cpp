#include "RigidBodyComponent.hpp"

#include <btBulletDynamicsCommon.h>

Engine::RigidBodyComponent::~RigidBodyComponent()
{
    if (Shape)
    {
        delete Shape;
        Shape = nullptr;
    }
}