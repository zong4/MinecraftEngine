#include "BVH.hpp"

#include "../Renderer/Librarys/ShaderLibrary.hpp"
#include "../Renderer/Librarys/VertexLibrary.hpp"

Engine::BVH::BVH(std::shared_ptr<Scene> scene)
{
    std::vector<Entity> entities;
    auto &&registry = scene->GetRegistry();
    auto &&view = registry.view<TransformComponent, MeshRendererComponent>();
    for (auto &entity : view)
    {
        entities.push_back(Entity{entity, &registry});

        // todo: move to other place
        // Transform the BoundingBox to world space
        registry.try_get<MeshRendererComponent>(entity)->WorldBBox =
            registry.try_get<MeshRendererComponent>(entity)->BBox.Transform(
                registry.try_get<TransformComponent>(entity)->GetTransformMatrix());
    }

    m_Root = Build(entities, 3);
}

Engine::BVH::~BVH()
{
    Destroy(m_Root);
    m_Root = nullptr;
}

void Engine::BVH::Render(int maxDepth) const
{
    auto &&shader = ShaderLibrary::GetInstance().GetShader("SimpleColor");
    shader->Bind();

    std::function<void(BVHNode *, int)> renderNode = [&](BVHNode *node, int depth) {
        if (!node || depth > maxDepth)
            return;

        // Calculate model matrix
        glm::vec3 scale = node->BBox.GetMax() - node->BBox.GetMin();
        glm::mat4 model = glm::translate(glm::mat4(1.0f), node->BBox.GetCenter());
        model = glm::scale(model, scale);
        shader->SetUniformMat4("uModel", model);

        // // Color by depth
        float t = depth / float(maxDepth);
        shader->SetUniformVec4("uColor", glm::vec4(t, 1.0f - t, 0.5f, 1.0f));

        // Render box
        VertexLibrary::GetInstance().GetVertex("Cube")->Render(RendererType::Lines);
        renderNode(node->Left, depth + 1);
        renderNode(node->Right, depth + 1);
    };

    renderNode(m_Root, 0);
    shader->Unbind();
}

Engine::BVHNode *Engine::BVH::Build(std::vector<Entity> entities, int leafSize)
{
    if (entities.empty())
        return nullptr;

    BVHNode *node = new BVHNode();

    // Calculate bounding box for current node
    BoundingBox box = entities[0].GetComponent<MeshRendererComponent>()->WorldBBox;
    for (size_t i = 1; i < entities.size(); i++)
        box = box.Merge(entities[i].GetComponent<MeshRendererComponent>()->WorldBBox);
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