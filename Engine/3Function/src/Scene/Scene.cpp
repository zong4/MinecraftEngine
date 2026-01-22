#include "Scene.hpp"

#include <btBulletDynamicsCommon.h>

Engine::Scene::~Scene()
{
    AudioSystem::GetInstance().Clear(m_Registry);
    NativeScriptSystem::GetInstance().Clear(m_Registry);
    m_Registry.clear();
}

Engine::Entity Engine::Scene::GetEntityByName(const std::string &name)
{
    auto &&view = m_Registry.view<LabelComponent>();
    for (auto &&entity : view)
    {
        auto &&nameComponent = view.get<LabelComponent>(entity);
        if (nameComponent.Name == name)
            return Entity(entity, &m_Registry);
    }
    static Entity nullEntity;
    return nullEntity;
}

void Engine::Scene::SetMainCamera(const Entity &camera)
{
    if (m_MainCamera)
        m_MainCamera.GetComponent<CameraComponent>()->Primary = false;
    m_MainCamera = camera;
    m_MainCamera.GetComponent<CameraComponent>()->Primary = true;
}

void Engine::Scene::Resize(int width, int height)
{
    PROFILE_FUNCTION();

    // Resize all cameras
    auto &&cameraView = m_Registry.view<CameraComponent>();
    for (auto &&entity : cameraView)
    {
        auto &&camera = cameraView.get<CameraComponent>(entity);
        camera.Resize(width, height);
    }

    // Resize color ID framebuffer
    m_ColorIDFrameBuffer->Resize(width, height);

    // Resize shadow map framebuffers for lights
    auto &&lightView = m_Registry.view<LightComponent>();
    for (auto &&entity : lightView)
    {
        auto &&lightComp = lightView.get<LightComponent>(entity);
        lightComp.ShadowMap->Resize(width, height);
    }
}

void Engine::Scene::Update(float deltaTime)
{
    PROFILE_FUNCTION();

    // Handle deleted entities
    for (auto &&entity : m_DeletedEntities)
    {
        if (auto &&rigidBody = entity.GetComponent<RigidBodyComponent>())
            m_PhysicSystem.DeleteRigidBody(rigidBody);
        if (auto &&audio = entity.GetComponent<AudioComponent>())
            audio->Stop();
        if (auto &&nativeScript = entity.GetComponent<NativeScriptComponent>())
            nativeScript->DestroyScript();
        m_Registry.destroy(entity.GetHandle());
    }
    m_DeletedEntities.clear();

    // Update Transform System
    TransformSystem::GetInstance().Update(m_Registry);
}

void Engine::Scene::UpdateRuntime(float deltaTime)
{
    PROFILE_FUNCTION();

    if (!m_Started)
    {
        Start();
        m_Started = true;
    }

    RendererSystem::GetInstance().Update(m_Registry);
    m_PhysicSystem.Update(m_Registry, deltaTime);
    ParticleSystem::GetInstance().Update(m_Registry, deltaTime);
    AudioSystem::GetInstance().Update(m_Registry);
    NativeScriptSystem::GetInstance().Update(m_Registry, deltaTime);
}

void Engine::Scene::DeleteEntity(const Entity &entity)
{
    PROFILE_FUNCTION();

    DeleteEntityReal(entity);
    RelationshipComponent::SetParentChild(Entity(), entity);
}

Engine::Entity Engine::Scene::AddEmptyEntity(const std::string &name, const TransformComponent &transform)
{
    PROFILE_FUNCTION();

    Entity entity{m_Registry.create(), &m_Registry};
    entity.AddComponent<LabelComponent>(name);
    entity.AddComponent<TransformComponent>(transform);
    entity.AddComponent<RelationshipComponent>();
    return entity;
}

Engine::Entity Engine::Scene::AddSquare(const std::string &name, const TransformComponent &transform,
                                        const SpriteRendererComponent &spriteRenderer,
                                        const MaterialComponent &materialComponent)
{
    Entity entity = AddEmptyEntity(name, transform);
    entity.AddComponent<SpriteRendererComponent>(spriteRenderer);
    entity.AddComponent<MaterialComponent>(materialComponent);
    return entity;
}

Engine::Entity Engine::Scene::AddCube(const std::string &name, const TransformComponent &transform,
                                      const MeshRendererComponent &meshRendererComponent,
                                      const MaterialComponent &materialComponent)
{
    Entity entity = AddEmptyEntity(name, transform);
    entity.AddComponent<MeshRendererComponent>(meshRendererComponent);
    entity.AddComponent<MaterialComponent>(materialComponent);
    return entity;
}

Engine::Entity Engine::Scene::AddCamera(const std::string &name, const TransformComponent &transform,
                                        const CameraComponent &cameraComponent)
{
    Entity entity = AddEmptyEntity(name, transform);
    entity.AddComponent<CameraComponent>(cameraComponent);
    return entity;
}

Engine::Entity Engine::Scene::AddLight(const std::string &name, const TransformComponent &transform,
                                       const LightComponent &lightComponent)
{
    Entity entity = AddEmptyEntity(name, transform);
    entity.AddComponent<LightComponent>(lightComponent);
    return entity;
}

void Engine::Scene::DeleteEntityReal(const Entity &entity)
{
    if (!entity)
        return;

    // Recursively delete children
    if (auto &&relationship = entity.GetComponent<RelationshipComponent>())
    {
        for (auto &&child : relationship->GetChildren())
            DeleteEntityReal(child);
    }

    // Mark entity for deletion at the end of the frame
    m_DeletedEntities.push_back(entity);
}
