#pragma once

#include "../EC/Components/CameraComponent.hpp"
#include "../EC/Components/LabelComponent.hpp"
#include "../EC/Components/LightComponent.hpp"
#include "../EC/Components/MaterialComponent.hpp"
#include "../EC/Components/NativeScriptComponent.hpp"
#include "../EC/Components/PhysicComponents.hpp"
#include "../EC/Components/RelationshipComponent.hpp"
#include "../EC/Components/RendererComponents.hpp"
#include "../EC/Components/TransformComponent.hpp"
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

    // Physics
    btDefaultCollisionConfiguration *m_CollisionConfiguration = new btDefaultCollisionConfiguration();
    btCollisionDispatcher *m_Dispatcher = new btCollisionDispatcher(m_CollisionConfiguration);
    btBroadphaseInterface *m_Broadphase = new btDbvtBroadphase();
    btSequentialImpulseConstraintSolver *m_Solver = new btSequentialImpulseConstraintSolver();
    btDiscreteDynamicsWorld *m_DynamicsWorld =
        new btDiscreteDynamicsWorld(m_Dispatcher, m_Broadphase, m_Solver, m_CollisionConfiguration);

private:
    void DeleteEntityReal(const Entity &entity);
};

} // namespace Engine