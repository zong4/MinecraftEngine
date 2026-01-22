#pragma once

#include "../Component/RelationshipComponent.hpp"
#include "../Component/TransformComponent.hpp"

namespace Engine
{

class TransformSystem
{
public:
    static TransformSystem &GetInstance();

public:
    void UpdateTransform(RelationshipComponent &relationship, TransformComponent &transform);

private:
    TransformSystem() = default;
    ~TransformSystem() = default;
};

} // namespace Engine