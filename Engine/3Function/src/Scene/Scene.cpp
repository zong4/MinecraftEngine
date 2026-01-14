#include "Scene.hpp"

Engine::Scene::~Scene()
{
    // World cleanup
    m_Registry.view<Engine::NativeScriptComponent>().each([&](auto &&entity, auto &&nsc) { nsc.DestroyScript(); });
    m_Registry.clear();

    // Physics cleanup
    delete m_DynamicsWorld;
    m_DynamicsWorld = nullptr;
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

    // Update all transform matrices
    auto &&entityView = m_Registry.view<TransformComponent, RelationshipComponent>();
    for (auto &&entity : entityView)
    {
        auto &&[transform, relationship] = entityView.get<TransformComponent, RelationshipComponent>(entity);
        if (!relationship.GetParent())
            transform.UpdateTransformMatrix(glm::mat4(1.0f), glm::quat(1.0f, 0.0f, 0.0f, 0.0f), &relationship);
    }

    // Update physics
    m_DynamicsWorld->stepSimulation(deltaTime, 10);
    auto &&rigibodyView = m_Registry.view<TransformComponent, RigidBodyComponent>();
    for (auto &&entity : rigibodyView)
    {
        auto &&[transform, rigibody] = rigibodyView.get<TransformComponent, RigidBodyComponent>(entity);
        btTransform btTransform;
        rigibody.body->getMotionState()->getWorldTransform(btTransform);

        glm::vec3 position(btTransform.getOrigin().getX(), btTransform.getOrigin().getY(),
                           btTransform.getOrigin().getZ());
        transform.Position = position;

        glm::quat rotation(btTransform.getRotation().getW(), btTransform.getRotation().getX(),
                           btTransform.getRotation().getY(), btTransform.getRotation().getZ());
        transform.SetRotationRadians(glm::eulerAngles(rotation));
    }

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

Engine::Entity Engine::Scene::Add2DObject(const std::string &name, const TransformComponent &transform,
                                          const SpriteRendererComponent &spriteRenderer)
{
    Entity entity = AddEmptyEntity(name, transform);
    entity.AddComponent<SpriteRendererComponent>(spriteRenderer);
    return entity;
}

Engine::Entity Engine::Scene::Add3DObject(const std::string &name, const TransformComponent &transform,
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
