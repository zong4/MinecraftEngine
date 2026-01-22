#pragma once

#include "../AssetsManager/ShadersManager.hpp"
#include "../ECS/Component/CameraComponent.hpp"
#include "../ECS/Component/LabelComponent.hpp"
#include "../ECS/Component/LightComponent.hpp"
#include "../ECS/Component/MaterialComponent.hpp"
#include "../ECS/Component/NativeScriptComponent.hpp"
#include "../ECS/Component/RelationshipComponent.hpp"
#include "../ECS/Component/RendererComponents.hpp"
#include "../ECS/Component/TransformComponent.hpp"
#include "../ECS/System/ParticleSystem.hpp"
#include "../ECS/System/PhysicSystem.hpp"

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
    void UpdateRuntime(float deltaTime);
    virtual void Render(const Entity &camera);
    virtual void Resize(int width, int height);

    // Entity management
    void DeleteEntity(const Entity &entity);
    Entity AddEmptyEntity(const std::string &name, const TransformComponent &transform = TransformComponent());
    Entity AddSquare(const std::string &name, const TransformComponent &transform,
                     const SpriteRendererComponent &spriteRenderer = SpriteRendererComponent());
    Entity AddCube(const std::string &name, const TransformComponent &transform,
                   const MeshRendererComponent &meshRendererComponent = MeshRendererComponent(),
                   const MaterialComponent &materialComponent = MaterialComponent(),
                   const RigidBodyComponent &rigidBodyComponent = RigidBodyComponent());
    Entity AddCamera(const std::string &name, const TransformComponent &transform,
                     const CameraComponent &cameraComponent);
    Entity AddLight(const std::string &name, const TransformComponent &transform, const LightComponent &lightComponent);

protected:
    std::string m_Name;
    entt::registry m_Registry = {};
    std::shared_ptr<Engine::FrameBuffer> m_ColorIDFrameBuffer =
        Engine::FrameBuffer::Create(Engine::Texture2DType::Integer, 1280, 720);

protected:
    virtual void RenderColorID() const = 0;
    void RenderParticles();

private:
    Entity m_MainCamera;
    std::vector<Entity> m_DeletedEntities = {};

    // Systems
    PhysicSystem m_PhysicSystem;

private:
    void UpdateTransformSystem(float deltaTime);
    void UpdatePhysicSystem(float deltaTime);
    void UpdateParticleSystem(float deltaTime);
    void UpdateScriptSystem(float deltaTime);
    void DeleteEntityReal(const Entity &entity);
};

} // namespace Engine