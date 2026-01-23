#include "BVH.hpp"

#include "../../ECS/Component/RendererComponents.hpp"
#include "../../ECS/Component/TransformComponent.hpp"

Engine::BVH::~BVH()
{
    Destroy(m_Root);
    m_Root = nullptr;
}

void Engine::BVH::Update(entt::registry &registry, int leafSize)
{
    // Gather all entities with MeshRendererComponent
    std::vector<Entity> entities;
    auto &&view = registry.view<TransformComponent, MeshRendererComponent>();
    for (auto &&entity : view)
    {
        entities.push_back(Entity{entity, &registry});
    }

    m_Root = Build(entities, leafSize);
}

Engine::BVHNode *Engine::BVH::Build(std::vector<Entity> &entities, int leafSize)
{
    if (entities.empty())
        return nullptr;

    BVHNode *node = new BVHNode();

    // Calculate bounding box for current node
    BoundingBox box = entities[0].GetComponent<MeshRendererComponent>()->WorldBBox;
    for (size_t i = 1; i < entities.size(); i++)
        box = box.Extend(entities[i].GetComponent<MeshRendererComponent>()->WorldBBox);
    node->BBox = box;

    // Stop if <= leaf size
    if (entities.size() <= leafSize)
    {
        node->Entities = entities;
        return node;
    }

    // Split entities
    int axis = box.GetLongestAxis();
    std::sort(entities.begin(), entities.end(), [axis](Entity a, Entity b) {
        return a.GetComponent<MeshRendererComponent>()->WorldBBox.GetCenter()[axis] <
               b.GetComponent<MeshRendererComponent>()->WorldBBox.GetCenter()[axis];
    });
    size_t mid = entities.size() / 2;
    std::vector<Entity> leftEntities(entities.begin(), entities.begin() + mid);
    std::vector<Entity> rightEntities(entities.begin() + mid, entities.end());

    // Recursively build child nodes
    node->Left = Build(leftEntities, leafSize);
    node->Right = Build(rightEntities, leafSize);
    return node;
}

void Engine::BVH::Destroy(BVHNode *node)
{
    if (!node)
        return;

    Destroy(node->Left);
    Destroy(node->Right);
    delete node;
}