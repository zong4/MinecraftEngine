#pragma once

#include "../AssetsManager/MaterialsManager.hpp"
#include "../AssetsManager/TexturesManager.hpp"
#include "../ECS/Component/LabelComponent.hpp"
#include "../ECS/System/AudioSystem.hpp"
#include "../ECS/System/ColliderSystem.hpp"
#include "../ECS/System/ParticleSystem.hpp"
#include "../ECS/System/PhysicSystem.hpp"
#include "../ECS/System/RendererSystem.hpp"
#include "../ECS/System/ScriptsSystem.hpp"
#include "../ECS/System/TransformSystem.hpp"

namespace Engine
{

class Scene
{
public:
    Scene(const std::string &name = "Untitled");
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
    virtual void Resize(int width, int height) { m_RendererSystem.Resize(m_Registry, width, height); }
    void Update(float deltaTime);
    void UpdateRuntime(float deltaTime);
    virtual void Render(const Entity &camera);

    // Entity management
    void DeleteEntity(const Entity &entity);
    Entity AddEmptyEntity(const std::string &name, const TransformComponent &transform = TransformComponent());
    Entity AddSquare(const std::string &name, const TransformComponent &transform,
                     const SpriteRendererComponent &spriteRenderer = SpriteRendererComponent(),
                     const MaterialComponent &materialComponent =
                         MaterialComponent(MaterialsManager::GetInstance().GetMaterial("Default2D")));
    Entity AddCube(const std::string &name, const TransformComponent &transform,
                   const MeshRendererComponent &meshRendererComponent = MeshRendererComponent(),
                   const MaterialComponent &materialComponent =
                       MaterialComponent(MaterialsManager::GetInstance().GetMaterial("Default3D")));
    Entity AddCamera(const std::string &name, const TransformComponent &transform,
                     const CameraComponent &cameraComponent);
    Entity AddLight(const std::string &name, const TransformComponent &transform, const LightComponent &lightComponent);

protected:
    RendererSystem m_RendererSystem;

private:
    // Scene management
    std::string m_Name;
    Entity m_MainCamera;
    entt::registry m_Registry = {};
    std::vector<Entity> m_DeleteEntities = {};

    // Systems
    bool m_Initialized = false;
    TransformSystem m_TransformSystem;
    ScriptsSystem m_ScriptsSystem;
    ColliderSystem m_ColliderSystem;
    PhysicSystem m_PhysicSystem;
    ParticleSystem m_ParticleSystem;
    AudioSystem m_AudioSystem;

private:
    void StackDeleteEntityRecursive(const Entity &entity);
    void DeleteEntityReal();
};

} // namespace Engine