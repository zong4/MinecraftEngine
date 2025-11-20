#pragma once

#include "Scene/Scene.hpp"

namespace MCEngine
{

struct BVHNode
{
    BoundingBox Box;
    std::vector<Entity> Entities;

    // Child nodes
    BVHNode *Left;
    BVHNode *Right;
};

class BVH
{
public:
    BVH(std::shared_ptr<Scene> scene);
    ~BVH();

public:
    void Render(int maxDepth) const;

private:
    BVHNode *m_Root;

private:
    BVHNode *Build(std::vector<Entity> entities, int leafSize);
    void Destroy(BVHNode *node);
};

} // namespace MCEngine
