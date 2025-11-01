#pragma once

#include "ECS/Entity/Entity.hpp"

#include "ECS/Component/Basic/BasicComponent.hpp"
#include "ECS/Component/Camera/CameraComponent.hpp"
#include "ECS/Component/Light/LightComponent.hpp"
#include "ECS/Component/Light/SkyboxComponent.hpp"
#include "ECS/Component/Renderer/RendererComponent.hpp"

namespace MCEngine
{

class Scene
{
public:
    Scene(const std::string &name = "Untitled") : m_Name(name) {}
    virtual ~Scene();

    // Getters
    const std::string &GetName() const { return m_Name; }
    entt::registry &GetRegistry() { return m_Registry; }
    Entity GetMainCamera() const { return m_MainCamera; }

    // Setters
    void SetName(const std::string &name) { m_Name = name; }
    void SetMainCamera(const Entity &camera)
    {
        if (m_MainCamera)
            m_MainCamera.GetComponent<CameraComponent>()->Primary = false;
        m_MainCamera = camera;
        m_MainCamera.GetComponent<CameraComponent>()->Primary = true;
    }

public:
    // Main loop
    void Update(float deltaTime);
    void PreRender();
    void RenderShadowMap() const;
    void Render(const Entity &camera) const;
    void RenderColorID() const;

    void Resize(float width, float height);

    // Entity management
    void DeleteEntity(const Entity &entity);
    Entity AddEmptyEntity(const std::string &name, const TransformComponent &transform = TransformComponent());
    Entity Add2DObject(const std::string &name, const TransformComponent &transform,
                       const SpriteRendererComponent &spriteRenderer = SpriteRendererComponent());
    Entity Add3DObject(const std::string &name, const TransformComponent &transform,
                       const MeshRendererComponent &meshRenderer = MeshRendererComponent());
    Entity AddCamera(const std::string &name, const TransformComponent &transform,
                     const CameraComponent &cameraComponent);
    Entity AddLight(const std::string &name, const TransformComponent &transform, const LightComponent &lightComponent);
    Entity AddSkybox(const std::string &name, const SkyboxComponent &skyboxComponent);

protected:
    std::string m_Name;

    // Scene data
    entt::registry m_Registry = {};
    Entity m_MainCamera;
    size_t m_SquaresCount = 0;
    size_t m_CubesCount = 0;

    // Lighting
    std::unique_ptr<MCEngine::FrameBuffer> m_ShadowMap =
        std::make_unique<MCEngine::FrameBuffer>(MCEngine::FrameBufferType::Depth, 1024, 1024);

protected:
    void Render2D() const;
    void Render3D() const;
    void RenderSkybox() const;
};

} // namespace MCEngine