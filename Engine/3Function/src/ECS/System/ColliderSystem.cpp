#include "ColliderSystem.hpp"

#include "../../AssetsManager/ShadersManager.hpp"
#include "../../Renderer/Library/VertexLibrary.hpp"

void Engine::ColliderSystem::Update(entt::registry &registry)
{
    PROFILE_FUNCTION();

    // Sprites
    auto &&spriteView = registry.view<TransformComponent, SpriteRendererComponent>();
    for (auto &&entity : spriteView)
    {
        auto &&[transform, spriteRenderer] = spriteView.get<TransformComponent, SpriteRendererComponent>(entity);
        spriteRenderer.WorldBBox = spriteRenderer.GetBBox().Transform(transform.GetTransformMatrix());
    }

    // Meshes
    auto &&meshView = registry.view<TransformComponent, MeshRendererComponent>();
    for (auto &&entity : meshView)
    {
        auto &&[transform, meshRenderer] = meshView.get<TransformComponent, MeshRendererComponent>(entity);
        meshRenderer.WorldBBox = meshRenderer.GetBBox().Transform(transform.GetTransformMatrix());
    }

    // Build BVH
    m_BVH = std::make_shared<BVH>(registry, 3);
}

void Engine::ColliderSystem::RenderBVH(entt::registry &registry, int maxDepth) const
{
    PROFILE_FUNCTION();

    if (!m_BVH)
        return;

    auto &&shader = ShadersManager::GetInstance().GetShader("SimpleColor");
    shader->Bind();

    // Recursive render function
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

    renderNode(m_BVH->GetRoot(), 0);
    shader->Unbind();
}