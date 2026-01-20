#pragma once

#include "../ECS/Components/CameraComponent.hpp"
#include "../ECS/Components/LabelComponent.hpp"
#include "../ECS/Components/LightComponent.hpp"
#include "../ECS/Components/MaterialComponent.hpp"
#include "../ECS/Components/NativeScriptComponent.hpp"
#include "../ECS/Components/RelationshipComponent.hpp"
#include "../ECS/Components/RendererComponents.hpp"
#include "../ECS/Components/TransformComponent.hpp"
#include "../ECS/Systems/ParticleSystem/ParticleSystem.hpp"
#include "../ECS/Systems/PhysicSystem/PhysicSystem.hpp"
#include "../Renderer/Librarys/ShaderLibrary.hpp"

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

private:
    Entity m_MainCamera;
    std::vector<Entity> m_DeletedEntities = {};

    // Systems
    PhysicSystem m_PhysicSystem;

private:
    void UpdateTransformSystem(float deltaTime);
    void UpdatePhysicSystem(float deltaTime);
    void UpdateScriptSystem(float deltaTime);
    void DeleteEntityReal(const Entity &entity);
};

} // namespace Engine