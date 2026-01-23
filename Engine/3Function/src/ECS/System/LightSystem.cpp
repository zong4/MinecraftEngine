#include "LightSystem.hpp"

#include "../../AssetsManager/ShadersManager.hpp"
#include "../../Renderer/Library/VertexLibrary.hpp"
#include "RendererSystem.hpp"

Engine::LightSystem &Engine::LightSystem::GetInstance()
{
    static LightSystem instance;
    return instance;
}

void Engine::LightSystem::Resize(entt::registry &registry, int width, int height)
{
    PROFILE_FUNCTION();

    // Resize shadow map framebuffers for lights
    auto &&view = registry.view<LightComponent>();
    for (auto &&entity : view)
    {
        auto &&light = view.get<LightComponent>(entity);
        light.ShadowMap->Resize(width, height);
    }
}

void Engine::LightSystem::Render(entt::registry &registry) const
{
    PROFILE_FUNCTION();

    RendererCommand::SetFaceCulling(CullingFace::Front);
    auto &&shader = Engine::ShadersManager::GetInstance().GetShader("ShadowMap");
    shader->Bind();
    auto &&view = registry.view<Engine::TransformComponent, Engine::LightComponent>();
    for (auto &&entity : view)
    {
        auto &&[transform, light] = view.get<Engine::TransformComponent, Engine::LightComponent>(entity);

        // Render to shadow map
        light.ShadowMap->Bind();
        RendererCommand::ClearDepthBuffer();
        {
            shader->SetUniformMat4("u_LightView", glm::inverse(transform.GetTransformMatrix()));
            shader->SetUniformMat4("u_LightProjection", glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 1.0f,
                                                                   20.0f)); // todo: calculate camera view

            // Render
            int cubesCount = RendererSystem::GetInstance().GetCubesCount();
            if (cubesCount > 0)
                VertexLibrary::GetInstance().GetVertex("Cubes")->Render(Engine::RendererType::Triangles,
                                                                        cubesCount * 36);
        }
        light.ShadowMap->Unbind();
    }
    shader->Unbind();
    RendererCommand::SetFaceCulling(CullingFace::Back);
}
