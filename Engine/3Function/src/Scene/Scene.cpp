#include "Scene.hpp"

#include "../Renderer/Librarys/UniformLibrary.hpp"

Engine::Scene::~Scene()
{
    m_Registry.view<Engine::NativeScriptComponent>().each([&](auto &&entity, auto &&nsc) { nsc.DestroyScript(); });
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

    // Handle deleted entities
    for (auto &&entity : m_DeletedEntities)
    {
        // Call OnDestroy for NativeScriptComponent
        if (auto &&nativeScript = entity.GetComponent<NativeScriptComponent>())
            nativeScript->DestroyScript();
        m_Registry.destroy(entity.GetHandle());
    }
    m_DeletedEntities.clear();

    UpdateTransformSystem(deltaTime);
}

void Engine::Scene::UpdateRuntime(float deltaTime)
{
    PROFILE_FUNCTION();

    UpdatePhysicSystem(deltaTime);

    // todo:: check
    // Transform the BoundingBox to world space
    {
        auto &&spriteView = m_Registry.view<TransformComponent, SpriteRendererComponent>();
        for (auto &&entity : spriteView)
        {
            auto &&[transform, spriteRenderer] = spriteView.get<TransformComponent, SpriteRendererComponent>(entity);
            spriteRenderer.WorldBBox = spriteRenderer.GetBBox().Transform(transform.GetTransformMatrix());
        }

        auto &&meshView = m_Registry.view<TransformComponent, MeshRendererComponent>();
        for (auto &&entity : meshView)
        {
            auto &&[transform, meshRenderer] = meshView.get<TransformComponent, MeshRendererComponent>(entity);
            meshRenderer.WorldBBox = meshRenderer.GetBBox().Transform(transform.GetTransformMatrix());
        }
    }

    UpdateScriptSystem(deltaTime);
}

void Engine::Scene::Render(const Entity &camera)
{
    PROFILE_FUNCTION();

    // Update camera uniform buffer
    auto &&transform = camera.GetComponent<TransformComponent>();
    auto &&cameraComp = camera.GetComponent<CameraComponent>();
    if (transform && cameraComp)
    {
        cameraComp->UpdateProjectionMatrix();
        UniformLibrary::GetInstance().UpdateUniform(
            "UniformBuffer0",
            {
                {glm::value_ptr(glm::inverse(transform->GetTransformMatrix())), sizeof(glm::mat4), 0}, // View matrix
                {glm::value_ptr(cameraComp->GetProjectionMatrix()), sizeof(glm::mat4),
                 sizeof(glm::mat4)}, // Projection matrix
                {glm::value_ptr(transform->Position), sizeof(glm::vec3),
                 sizeof(glm::mat4) + sizeof(glm::mat4)}, // Camera position
            });
    }
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
                                        const SpriteRendererComponent &spriteRenderer)
{
    Entity entity = AddEmptyEntity(name, transform);
    entity.AddComponent<SpriteRendererComponent>(spriteRenderer);
    return entity;
}

Engine::Entity Engine::Scene::AddCube(const std::string &name, const TransformComponent &transform,
                                      const MeshRendererComponent &meshRendererComponent,
                                      const MaterialComponent &materialComponent,
                                      const RigidBodyComponent &rigidBodyComponent)
{
    Entity entity = AddEmptyEntity(name, transform);
    entity.AddComponent<MeshRendererComponent>(meshRendererComponent);
    entity.AddComponent<MaterialComponent>(materialComponent);
    entity.AddComponent<RigidBodyComponent>(rigidBodyComponent);
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

void Engine::Scene::UpdateTransformSystem(float deltaTime)
{
    PROFILE_FUNCTION();

    // Update all transform matrices
    auto &&entityView = m_Registry.view<TransformComponent, RelationshipComponent>();
    for (auto &&entity : entityView)
    {
        auto &&[transform, relationship] = entityView.get<TransformComponent, RelationshipComponent>(entity);
        if (!relationship.Parent)
            transform.UpdateTransformMatrix(glm::mat4(1.0f), glm::quat(1.0f, 0.0f, 0.0f, 0.0f), &relationship);
    }
}

void Engine::Scene::UpdatePhysicSystem(float deltaTime)
{
    PROFILE_FUNCTION();

    // Sync RigidBodies with TransformComponents
    auto &&view = m_Registry.view<TransformComponent, RigidBodyComponent>();
    for (auto &&entity : view)
    {
        auto &&[transform, rigidBody] = view.get<TransformComponent, RigidBodyComponent>(entity);
        m_PhysicSystem.UpdateRigidBody(rigidBody, transform);
    }

    // Update physics
    m_PhysicSystem.Update(deltaTime);

    // Sync TransformComponents with RigidBodies
    auto &&rigibodyView = m_Registry.view<TransformComponent, RigidBodyComponent>();
    for (auto &&entity : rigibodyView)
    {
        auto &&[transform, rigibody] = rigibodyView.get<TransformComponent, RigidBodyComponent>(entity);
        btTransform btTransform;
        rigibody.Body->getMotionState()->getWorldTransform(btTransform);

        glm::vec3 position(btTransform.getOrigin().getX(), btTransform.getOrigin().getY(),
                           btTransform.getOrigin().getZ());
        transform.Position = position;

        glm::quat rotation(btTransform.getRotation().getW(), btTransform.getRotation().getX(),
                           btTransform.getRotation().getY(), btTransform.getRotation().getZ());
        transform.Rotation = glm::eulerAngles(rotation);
    }
}

void Engine::Scene::UpdateScriptSystem(float deltaTime)
{
    PROFILE_FUNCTION();

    // Update all scripts
    m_Registry.view<Engine::NativeScriptComponent>().each([&](auto &&entity, auto &&nsc) {
        if (!nsc.Instance)
        {
            nsc.Instance = nsc.InstantiateScript();
            nsc.Instance->OnStart();
        }
        nsc.Instance->OnUpdate(deltaTime);
    });
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
