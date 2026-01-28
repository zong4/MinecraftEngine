#include "Scene.hpp"

#include "../Renderer/Library/UniformLibrary.hpp"

Engine::Scene::Scene(const std::string &name) : m_Name(name)
{
    auto particle = AddEmptyEntity("ParticleSystem");
    particle.AddComponent<ParticleComponent>([]() -> Particle {
        return Particle{
            glm::vec3(0.0f, 3.0f, 0.0f),
            glm::vec3(((rand() % 100) / 100.0f - 0.5f) * 2.0f, (rand() % 100) / 100.0f * 2.0f,
                      ((rand() % 100) / 100.0f - 0.5f) * 2.0f),
            2.0f,
        };
    });
}

Engine::Scene::~Scene()
{
    AudioSystem::GetInstance().Clear(m_Registry);
    NativeScriptSystem::GetInstance().Clear(m_Registry);
    m_LuaScriptSystem.Clear(m_Registry);
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

    CameraSystem::GetInstance().Resize(m_Registry, width, height);
    RendererSystem::GetInstance().Resize(width, height);
    LightSystem::GetInstance().Resize(m_Registry, width, height);
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

    // Update systems
    if (!m_Started)
    {
        NativeScriptSystem::GetInstance().Start(m_Registry);
        m_LuaScriptSystem.Start(m_Registry);
        m_Started = true;
    }
    TransformSystem::GetInstance().Update(m_Registry);
    RendererSystem::GetInstance().Update(m_Registry);
    ColliderSystem::GetInstance().Update(m_Registry);
}

void Engine::Scene::UpdateRuntime(float deltaTime)
{
    PROFILE_FUNCTION();

    // Update systems
    m_PhysicSystem.Update(m_Registry, deltaTime);
    ParticleSystem::GetInstance().Update(m_Registry, deltaTime);
    AudioSystem::GetInstance().Update(m_Registry);
    NativeScriptSystem::GetInstance().Update(m_Registry, deltaTime);
    m_LuaScriptSystem.Update(m_Registry, deltaTime);
}

void Engine::Scene::Render(const Entity &camera)
{
    PROFILE_FUNCTION();

    auto &&cameraComponent = camera.GetComponent<CameraComponent>();
    if (cameraComponent == nullptr)
        return;
    if (cameraComponent->GetWidth() == 0 || cameraComponent->GetHeight() == 0) // Minimize window
        return;

    // Upload camera data
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

    // Clear buffers
    RendererCommand::SetClearColor(cameraComponent->BackgroundColor);
    RendererCommand::Clear();

    // Render
    RendererSystem::GetInstance().Upload(m_Registry);
    LightSystem::GetInstance().Render(m_Registry);
    RendererSystem::GetInstance().Render(m_Registry);
    ColliderSystem::GetInstance().RenderBVH(m_Registry, 3);
    ParticleSystem::GetInstance().Render(m_Registry);
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
