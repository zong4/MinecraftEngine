#pragma once

#include "../../ECS/Entity/Entity.hpp"
#include "BoundingBox.hpp"

namespace Engine
{

struct BVHNode
{
    BoundingBox BBox;
    std::vector<Entity> Entities;

    // Child nodes
    BVHNode *Left = nullptr;
    BVHNode *Right = nullptr;
};

class BVH
{
public:
    BVH(entt::registry &registry, int leafSize) : m_Root(nullptr) { Update(registry, leafSize); }
    ~BVH();

    // Getters
    BVHNode *GetRoot() const { return m_Root; }

public:
    void Update(entt::registry &registry, int leafSize);

private:
    BVHNode *m_Root;

private:
    BVHNode *Build(std::vector<Entity> &entities, int leafSize);
    void Destroy(BVHNode *node);
};

} // namespace Engine
