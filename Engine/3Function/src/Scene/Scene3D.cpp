#include "Scene3D.hpp"

#include "../Physic/Ray/RayTracing.hpp"
#include "../Renderer/Library/VertexLibrary.hpp"

Engine::Scene3D::Scene3D(const std::string &name) : Scene(name) {}

void Engine::Scene3D::Render(const Entity &camera)
{
    PROFILE_FUNCTION();

    Scene::Render(camera);
    RendererSystem::GetInstance().RenderSkybox();

    // // Ray tracing
    // if (!m_RayTracingRunning.exchange(true))
    // {
    //     std::thread([this, camera] {
    //         std::vector<Entity> entities;
    //         auto &&registry = this->GetRegistry();
    //         auto &&view = registry.view<TransformComponent, MeshRendererComponent>();
    //         for (auto &&entity : view)
    //         {
    //             entities.push_back(Entity{entity, &registry});
    //         }
    //         std::vector<glm::vec4> tempBuffer;
    //         Engine::RayTracing::RenderScene(camera, entities, 100, 1, tempBuffer);
    //         m_RayTracingRunning = false;
    //     }).detach();
    // }
}
