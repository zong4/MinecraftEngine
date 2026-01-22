#pragma once

#include "../AssetsManager/MaterialsManager.hpp"
#include "../AssetsManager/ShadersManager.hpp"
#include "../AssetsManager/TexturesManager.hpp"
#include "../ECS/Component/CameraComponent.hpp"
#include "../ECS/Component/LabelComponent.hpp"
#include "../ECS/Component/LightComponent.hpp"
#include "../ECS/Component/MaterialComponent.hpp"
#include "../ECS/System/AudioSystem.hpp"
#include "../ECS/System/NativeScriptSystem.hpp"
#include "../ECS/System/ParticleSystem.hpp"
#include "../ECS/System/PhysicSystem.hpp"
#include "../ECS/System/RendererSystem.hpp"
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
    virtual void Resize(int width, int height);
    void Start() { NativeScriptSystem::GetInstance().Start(m_Registry); }
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
    std::shared_ptr<TextureCube> m_SkyboxTexture = TexturesManager::GetInstance().GetTextureCube("Default");

private:
    // Scene management
    std::string m_Name;
    Entity m_MainCamera;
    entt::registry m_Registry = {};
    std::vector<Entity> m_DeletedEntities = {};

    // Renderer
    int m_SquaresCount = 0;
    int m_CubesCount = 0;
    std::shared_ptr<FrameBuffer> m_ColorIDFrameBuffer = FrameBuffer::Create(Texture2DType::Integer, 1280, 720);

    // Physic
    PhysicSystem m_PhysicSystem;

    // Native script
    bool m_Started = false;

private:
    void UploadSquaresData();
    void UploadCubesData();
    void RenderShadowMap() const;
    void Render2D(const Entity &camera) const;
    void Render3D(const Entity &camera) const;
    void RenderColorID() const;
    void DeleteEntityReal(const Entity &entity);
};

} // namespace Engine