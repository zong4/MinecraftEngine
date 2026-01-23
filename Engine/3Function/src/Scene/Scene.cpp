#include "Scene.hpp"

#include "../Renderer/Library/UniformLibrary.hpp"
#include "../Renderer/Library/VertexLibrary.hpp"
#include <btBulletDynamicsCommon.h>

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

    // Update systems
    TransformSystem::GetInstance().Update(m_Registry);
    ColliderSystem::GetInstance().Update(m_Registry);
}

void Engine::Scene::UpdateRuntime(float deltaTime)
{
    PROFILE_FUNCTION();

    if (!m_Started)
    {
        Start();
        m_Started = true;
    }

    // Update systems
    RendererSystem::GetInstance().Update(m_Registry);
    m_PhysicSystem.Update(m_Registry, deltaTime);
    ParticleSystem::GetInstance().Update(m_Registry, deltaTime);
    AudioSystem::GetInstance().Update(m_Registry);
    NativeScriptSystem::GetInstance().Update(m_Registry, deltaTime);
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

    // Clear buffers
    RendererCommand::SetClearColor(camera.GetComponent<CameraComponent>()->BackgroundColor);
    RendererCommand::Clear();

    // Render 2D
    UploadSquaresData();
    Render2D(camera);

    // Render 3D
    UploadCubesData();
    RenderShadowMap();
    Render3D(camera);

    // Render Common
    ColliderSystem::GetInstance().RenderBVH(m_Registry, 3);
    ParticleSystem::GetInstance().Render(m_Registry);
    RenderColorID();
}

void Engine::Scene::UploadSquaresData()
{
    PROFILE_FUNCTION();

    int index = 0;
    std::vector<Vertex2D> vertices;
    std::vector<unsigned int> indices;
    auto &&view =
        m_Registry.view<Engine::TransformComponent, Engine::SpriteRendererComponent, Engine::MaterialComponent>();
    for (auto &&entity : view)
    {
        auto &&[transform, spriteRenderer, material] =
            view.get<Engine::TransformComponent, Engine::SpriteRendererComponent, Engine::MaterialComponent>(entity);

        // Vertices
        for (int i = 0; i < 4; i++)
        {
            glm::mat4 u_Model = transform.GetTransformMatrix();
            vertices.push_back({(uint32_t)entity + 1, glm::vec3(u_Model * glm::vec4(g_SquareData.Positions[i], 1.0f)),
                                g_SquareData.TexCoords[i],
                                TexturesManager::GetInstance().GetTextureSlot(
                                    material.GetProperty("Texture").GetValueAs<std::shared_ptr<Texture2D>>()),
                                material.GetProperty("Color").GetValueAs<glm::vec4>()});
        }

        // Indices
        for (int i = 0; i < 6; i++)
            indices.push_back(g_SquareData.Indices[i] + index * 4);

        index++;
    }

    // Update counts and buffers
    m_SquaresCount = index;
    VertexLibrary::GetInstance().GetVertex("Squares")->GetVertexBuffer()->SetData(
        vertices.data(), m_SquaresCount * 4 * sizeof(Vertex2D), 0);
    VertexLibrary::GetInstance().GetVertex("Squares")->GetIndexBuffer()->SetData(
        indices.data(), m_SquaresCount * 6 * sizeof(unsigned int), 0);
}

void Engine::Scene::UploadCubesData()
{
    PROFILE_FUNCTION();

    int index = 0;
    std::vector<Vertex3D> vertices;
    auto &&view =
        m_Registry.view<Engine::TransformComponent, Engine::MeshRendererComponent, Engine::MaterialComponent>();
    for (auto &&entity : view)
    {
        auto &&[transform, meshRenderer, material] =
            view.get<Engine::TransformComponent, Engine::MeshRendererComponent, Engine::MaterialComponent>(entity);

        // Get color property if available
        glm::vec4 color = glm::vec4(1.0f);
        if (auto &&colorProp = material.GetProperty("Color"))
        {
            if (colorProp.GetType() == MaterialPropertyType::Vec4)
                color = colorProp.GetValueAs<glm::vec4>();
            else if (colorProp.GetType() == MaterialPropertyType::Vec3)
                color = glm::vec4(colorProp.GetValueAs<glm::vec3>(), 1.0f);
        }

        // Get Blinn-Phong material properties if available
        glm::vec4 materialData = glm::vec4(0.0f);
        if (auto &&ambientStrengthProp = material.GetProperty("AmbientStrength"))
            materialData.x = ambientStrengthProp.GetValueAs<float>();
        if (auto &&diffuseStrengthProp = material.GetProperty("DiffuseStrength"))
            materialData.y = diffuseStrengthProp.GetValueAs<float>();
        if (auto &&specularStrengthProp = material.GetProperty("SpecularStrength"))
            materialData.z = specularStrengthProp.GetValueAs<float>();
        if (auto &&shininessProp = material.GetProperty("Shininess"))
            materialData.w = shininessProp.GetValueAs<float>();

        // Vertices
        for (int i = 0; i < 36; ++i)
        {
            glm::mat4 u_Model = transform.GetTransformMatrix();
            vertices.push_back(
                {(uint32_t)entity + 1, glm::vec3(u_Model * glm::vec4(g_CubeData.Positions[i], 1.0f)),
                 glm::normalize(glm::transpose(glm::inverse(glm::mat3(u_Model))) * g_CubeData.Normals[i]),
                 g_CubeData.Positions[i], color, materialData});
        }
        index++;
    }

    // Update count and buffer data
    m_CubesCount = index;
    VertexLibrary::GetInstance().GetVertex("Cubes")->GetVertexBuffer()->SetData(
        vertices.data(), m_CubesCount * 36 * sizeof(Vertex3D), 0);
}

void Engine::Scene::RenderShadowMap() const
{
    PROFILE_FUNCTION();

    RendererCommand::SetFaceCulling(CullingFace::Front);
    auto &&shader = Engine::ShadersManager::GetInstance().GetShader("ShadowMap");
    shader->Bind();
    auto &&lightView = m_Registry.view<Engine::TransformComponent, Engine::LightComponent>();
    for (auto &&lightEntity : lightView)
    {
        auto &&[transform, light] = lightView.get<Engine::TransformComponent, Engine::LightComponent>(lightEntity);

        // Render to shadow map
        light.ShadowMap->Bind();
        RendererCommand::ClearDepthBuffer();
        {
            shader->SetUniformMat4("u_LightView", glm::inverse(transform.GetTransformMatrix()));
            shader->SetUniformMat4("u_LightProjection", glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 1.0f,
                                                                   20.0f)); // todo: calculate camera view
            if (m_CubesCount > 0)
                VertexLibrary::GetInstance().GetVertex("Cubes")->Render(Engine::RendererType::Triangles,
                                                                        m_CubesCount * 36);
        }
        light.ShadowMap->Unbind();
    }
    shader->Unbind();
    RendererCommand::SetFaceCulling(CullingFace::Back);
}

void Engine::Scene::Render2D(const Entity &camera) const
{
    PROFILE_FUNCTION();

    // Shader is same for 2d sprites
    auto &&shader = Engine::ShadersManager::GetInstance().GetShader("Texture");
    shader->Bind();

    // Textures
    auto &&view = m_Registry.view<Engine::SpriteRendererComponent, Engine::MaterialComponent>();
    for (auto &&entity : view)
    {
        auto &&[spriteRenderer, material] =
            view.get<Engine::SpriteRendererComponent, Engine::MaterialComponent>(entity);
        auto &&texture = material.GetProperty("Texture").GetValueAs<std::shared_ptr<Texture2D>>();
        int texID = TexturesManager::GetInstance().GetTextureSlot(texture);
        if (texID != -1)
            texture->Active(texID);
    }

    // Render
    if (m_SquaresCount > 0)
        VertexLibrary::GetInstance().GetVertex("Squares")->Render(Engine::RendererType::Triangles, m_SquaresCount * 6);

    // Clear texture slots
    TexturesManager::GetInstance().ClearTextureSlots();
    shader->Unbind();
}

void Engine::Scene::Render3D(const Entity &camera) const
{
    PROFILE_FUNCTION();

    // Shader is same for 3D meshes
    auto &&shader = ShadersManager::GetInstance().GetShader("BlinnPhong");
    shader->Bind();

    // Light
    int lightIndex = 0;
    {
        // Initialize all lights as inactive
        for (int i = 0; i < 4; i++) // todo: max lights constant
            shader->SetUniformInt("u_Light[" + std::to_string(i) + "].Type", -1);

        // Upload active lights
        auto &&lightView = m_Registry.view<Engine::TransformComponent, Engine::LightComponent>();
        for (auto &&entity : lightView)
        {
            auto &&[transform, light] = lightView.get<Engine::TransformComponent, Engine::LightComponent>(entity);

            // Light properties
            shader->SetUniformInt("u_Light[" + std::to_string(lightIndex) + "].Type",
                                  static_cast<int>(light.GetType()));
            shader->SetUniformVec3("u_Light[" + std::to_string(lightIndex) + "].Position", transform.Position);
            shader->SetUniformVec3("u_Light[" + std::to_string(lightIndex) + "].Color", light.Color * light.Intensity);
            shader->SetUniformFloat("u_Light[" + std::to_string(lightIndex) + "].Constant", light.Constant);
            shader->SetUniformFloat("u_Light[" + std::to_string(lightIndex) + "].Linear", light.Linear);
            shader->SetUniformFloat("u_Light[" + std::to_string(lightIndex) + "].Quadratic", light.Quadratic);
            shader->SetUniformVec3("u_Light[" + std::to_string(lightIndex) + "].Direction",
                                   transform.GetForward(TransformSpace::Global));
            shader->SetUniformFloat("u_Light[" + std::to_string(lightIndex) + "].CutOff",
                                    glm::cos(glm::radians(light.InnerAngle)));
            shader->SetUniformFloat("u_Light[" + std::to_string(lightIndex) + "].OuterCutOff",
                                    glm::cos(glm::radians(light.OuterAngle)));

            // Shadow
            shader->SetUniformMat4("u_LightView[" + std::to_string(lightIndex) + "]",
                                   glm::inverse(transform.GetTransformMatrix()));
            shader->SetUniformMat4(
                "u_LightProjection[" + std::to_string(lightIndex) + "]",
                glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 1.0f, 20.0f)); // todo: calculate camera view
            shader->SetUniformInt("u_ShadowMap[" + std::to_string(lightIndex) + "]", lightIndex);
            light.ShadowMap->GetTexture()->Active(lightIndex);

            lightIndex++;
        }
        shader->SetUniformInt("u_NumLights", lightIndex);
    }

    // Textures
    TexturesManager::GetInstance().GetTextureCube("GrassBlock")->Active(lightIndex + 1);
    shader->SetUniformInt("u_Texture", lightIndex + 1);

    // Skybox
    shader->SetUniformInt("u_Skybox", lightIndex);
    m_SkyboxTexture->Active(lightIndex);

    // Render
    if (m_CubesCount > 0)
        VertexLibrary::GetInstance().GetVertex("Cubes")->Render(Engine::RendererType::Triangles, m_CubesCount * 36);

    // Clear texture slots
    TexturesManager::GetInstance().ClearTextureSlots();
    shader->Unbind();
}

void Engine::Scene::RenderColorID() const
{
    PROFILE_FUNCTION();

    m_ColorIDFrameBuffer->Bind();
    Engine::RendererCommand::Clear();
    auto &&shader = Engine::ShadersManager::GetInstance().GetShader("ColorIDPicking");
    shader->Bind();

    // Render entity IDs as color IDs
    if (m_SquaresCount > 0)
        VertexLibrary::GetInstance().GetVertex("Squares")->Render(Engine::RendererType::Triangles, m_SquaresCount * 6);
    if (m_CubesCount > 0)
        VertexLibrary::GetInstance().GetVertex("Cubes")->Render(Engine::RendererType::Triangles, m_CubesCount * 36);

    shader->Unbind();
    m_ColorIDFrameBuffer->Unbind();
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
