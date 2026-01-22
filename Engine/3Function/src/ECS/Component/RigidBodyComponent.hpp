#pragma once

#include <Platform.hpp>

class btCollisionShape;
class btRigidBody;

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
    ~RigidBodyComponent();

    // Copy and move semantics
    // RigidBodyComponent(const RigidBodyComponent &) = delete;
    // RigidBodyComponent &operator=(const RigidBodyComponent &) = delete;
};

} // namespace Engine