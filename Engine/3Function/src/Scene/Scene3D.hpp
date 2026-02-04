#pragma once

#include "../ECS/System/RayTracingSystem.hpp"
#include "Scene.hpp"

namespace Engine
{

class Scene3D : public Scene
{
public:
    Scene3D(const std::string &name = "Untitled") : Scene(name) {}
    virtual ~Scene3D() override = default;

public:
    void Render(const Entity &camera) override
    {
        PROFILE_FUNCTION();

        Scene::Render(camera);
        m_RendererSystem.RenderSkybox();
        // RayTracingSystem::GetInstance().Render(GetRegistry(), camera, 25, 1);
    }
};

} // namespace Engine
