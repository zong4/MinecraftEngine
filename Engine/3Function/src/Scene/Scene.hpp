#pragma once

#include "../ECS/Components/BasicComponents.hpp"
#include "../ECS/Components/CameraComponent.hpp"
#include "../ECS/Components/LightComponent.hpp"
#include "../ECS/Components/MaterialComponent.hpp"
#include "../ECS/Components/RendererComponents.hpp"
#include "../ECS/Components/SkyboxComponent.hpp"
#include "../Renderers/ParticleSystem/ParticleSystem.hpp"

namespace Engine
{

class Scene
{
public:
    Scene(const std::string &name = "Untitled") : m_Name(name) {}
    virtual ~Scene();

    // Getters
    const std::string &GetName() const { return m_Name; }
    const Entity &GetMainCamera() const { return m_MainCamera; }
    entt::registry &GetRegistry() { return m_Registry; }
    Entity GetEntityByName(const std::string &name);

    // Setters
    void SetName(const std::string &name) { m_Name = name; }
    void SetMainCamera(const Entity &camera);

public:
    void Update(float deltaTime);
    virtual void Render(const Entity &camera) = 0;
    virtual void Resize(int width, int height) = 0;

    // Entity management
    void DeleteEntity(const Entity &entity);
    Entity AddEmptyEntity(const std::string &name, const TransformComponent &transform = TransformComponent());
    Entity Add2DObject(const std::string &name, const TransformComponent &transform,
                       const SpriteRendererComponent &spriteRenderer = SpriteRendererComponent());
    Entity Add3DObject(const std::string &name, const TransformComponent &transform,
                       const MaterialComponent &materialComponent = MaterialComponent(),
                       const MeshRendererComponent &meshRendererComponent = MeshRendererComponent());
    Entity AddCamera(const std::string &name, const TransformComponent &transform,
                     const CameraComponent &cameraComponent);
    Entity AddLight(const std::string &name, const TransformComponent &transform, const LightComponent &lightComponent);
    Entity AddSkybox(const std::string &name, const SkyboxComponent &skyboxComponent);

protected:
    std::string m_Name;
    entt::registry m_Registry = {};
    std::shared_ptr<Engine::FrameBuffer> m_ColorIDFrameBuffer =
        Engine::FrameBuffer::Create(Engine::Texture2DType::Integer, 1280, 720);

protected:
    virtual void RenderColorID() const = 0;

private:
    Entity m_MainCamera;
    std::vector<Entity> m_DeletedEntities = {};

private:
    void DeleteEntityReal(const Entity &entity);
};

} // namespace Engine