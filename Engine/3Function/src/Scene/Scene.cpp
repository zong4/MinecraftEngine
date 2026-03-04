#include "Scene.hpp"

#include "../Renderer/Library/UniformLibrary.hpp"

Engine::Scene::Scene(const std::string &name) : m_Name(name) {}

Engine::Scene::~Scene()
{
    m_AudioSystem.Shutdown(m_Registry);
    m_PhysicSystem.Shutdown();
    m_ScriptsSystem.Shutdown(m_Registry);
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

void Engine::Scene::Update(float deltaTime)
{
    PROFILE_FUNCTION();

    // Delete marked entities
    DeleteEntityReal();

    // Update systems
    if (!m_Initialized)
    {
        m_ScriptsSystem.Init(m_Registry);
        m_Initialized = true;
    }
    m_TransformSystem.Update(m_Registry);
    // m_ColliderSystem.Update(m_Registry);
}

void Engine::Scene::UpdateRuntime(float deltaTime)
{
    PROFILE_FUNCTION();

    // Update systems for runtime
    m_PhysicSystem.Update(m_Registry, deltaTime);
    m_ParticleSystem.Update(m_Registry, deltaTime);
    m_AudioSystem.Update(m_Registry);
    m_ScriptsSystem.Update(m_Registry, deltaTime);
}

void Engine::Scene::Render(const Entity &camera, const std::shared_ptr<FrameBuffer> &colorIDFrameBuffer)
{
    PROFILE_FUNCTION();

    auto &&cameraComponent = camera.GetComponent<CameraComponent>();
    if (cameraComponent == nullptr)
        return;
    if (cameraComponent->GetWidth() == 0 || cameraComponent->GetHeight() == 0) // Minimize window
        return;

    // Update main camera uniforms
    auto &&transform = camera.GetComponent<TransformComponent>();
    cameraComponent->UpdateProjectionMatrix(); // Ensure projection matrix is updated
    UniformLibrary::GetInstance().UpdateUniform(
        "UniformBuffer0",
        {
            {glm::value_ptr(glm::inverse(transform->GetTransformMatrix())), sizeof(glm::mat4), 0}, // View matrix
            {glm::value_ptr(cameraComponent->GetProjectionMatrix()), sizeof(glm::mat4),
             sizeof(glm::mat4)}, // Projection matrix
            {glm::value_ptr(transform->Position), sizeof(glm::vec3),
             sizeof(glm::mat4) + sizeof(glm::mat4)}, // Camera position
        });

    // Clear screen
    RendererCommand::SetClearColor(cameraComponent->BackgroundColor);
    RendererCommand::Clear();

    // Render scene
    m_RendererSystem.Render(m_Registry, colorIDFrameBuffer);
    // m_ColliderSystem.RenderBVH(m_Registry, 3);
    m_ParticleSystem.Render(m_Registry);
}

void Engine::Scene::DeleteEntity(const Entity &entity)
{
    PROFILE_FUNCTION();

    StackDeleteEntityRecursive(entity);
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
                                        const MaterialComponent &materialComponent)
{
    Entity entity = AddEmptyEntity(name, transform);
    entity.AddComponent<SpriteRendererComponent>();
    entity.AddComponent<MaterialComponent>(materialComponent);
    return entity;
}

Engine::Entity Engine::Scene::AddCube(const std::string &name, const TransformComponent &transform,
                                      const MaterialComponent &materialComponent)
{
    Entity entity = AddEmptyEntity(name, transform);
    entity.AddComponent<MeshRendererComponent>();
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

void Engine::Scene::StackDeleteEntityRecursive(const Entity &entity)
{
    if (!entity)
        return;

    // Recursively delete children
    if (auto &&relationship = entity.GetComponent<RelationshipComponent>())
    {
        for (auto &&child : relationship->GetChildren())
            StackDeleteEntityRecursive(child);
    }

    // Mark entity for deletion at the end of the frame
    m_DeleteEntities.push_back(entity);
}

void Engine::Scene::DeleteEntityReal()
{
    PROFILE_FUNCTION();

    for (auto &&entity : m_DeleteEntities)
    {
        m_AudioSystem.Delete(entity);
        m_PhysicSystem.Delete(entity);
        m_ScriptsSystem.Delete(entity);
        m_Registry.destroy(entity.GetHandle());
    }
    m_DeleteEntities.clear();
}