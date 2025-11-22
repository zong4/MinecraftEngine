#include "Scene.hpp"

#include "../Renderers/Librarys/ShaderLibrary.hpp"
#include "../Renderers/Librarys/UniformLibrary.hpp"
#include "../Renderers/Librarys/VertexLibrary.hpp"
#include "../Renderers/Material/MaterialLibrary.hpp"

Engine::Scene::~Scene()
{
    m_Registry.view<Engine::NativeScriptComponent>().each([&](auto &&entity, auto &&nsc) { nsc.DestroyScript(); });
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
    auto &&view = m_Registry.view<TransformComponent, RelationshipComponent>();
    for (auto &&entity : view)
    {
        auto &&[transform, relationship] = view.get<TransformComponent, RelationshipComponent>(entity);
        if (!relationship.GetParent())
            transform.UpdateTransformMatrix(glm::mat4(1.0f), glm::quat(1.0f, 0.0f, 0.0f, 0.0f), &relationship);
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

void Engine::Scene::Render(const Entity &camera)
{
    PROFILE_FUNCTION();

    UploadSquaresData();
    UploadCubesData();

    // Clear buffers
    {
        RendererCommand::SetClearColor(camera.GetComponent<CameraComponent>()->BackgroundColor);
        RendererCommand::Clear();
    }

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

    Render2D();
    RenderShadowMap();
    Render3D();
    RenderSkybox();
    RenderColorID();
}

void Engine::Scene::Resize(float width, float height)
{
    PROFILE_FUNCTION();

    m_ViewportWidth = static_cast<uint32_t>(width);
    m_ViewportHeight = static_cast<uint32_t>(height);

    auto &&view = m_Registry.view<CameraComponent>();
    for (auto &&entity : view)
    {
        auto &&camera = view.get<CameraComponent>(entity);
        camera.Resize(width, height);
    }
}

void Engine::Scene::DeleteEntity(const Entity &entity)
{
    if (!entity)
        return;

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
                                          const MaterialComponent &materialComponent,
                                          const MeshRendererComponent &meshRendererComponent)
{
    Entity entity = AddEmptyEntity(name, transform);
    entity.AddComponent<MaterialComponent>(materialComponent);
    entity.AddComponent<MeshRendererComponent>(meshRendererComponent);
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

Engine::Entity Engine::Scene::AddSkybox(const std::string &name, const SkyboxComponent &skyboxComponent)
{
    Entity entity = AddEmptyEntity(name);
    entity.AddComponent<SkyboxComponent>(skyboxComponent);
    return entity;
}

void Engine::Scene::DeleteEntityReal(const Entity &entity)
{
    PROFILE_FUNCTION();

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

void Engine::Scene::UploadSquaresData()
{
    PROFILE_FUNCTION();

    int index = 0;
    std::vector<Vertex2D> vertices;
    std::vector<unsigned int> indices;
    auto &&view = m_Registry.view<Engine::TransformComponent, Engine::SpriteRendererComponent>();
    for (auto &&entity : view)
    {
        auto &&[transform, sprite] = view.get<Engine::TransformComponent, Engine::SpriteRendererComponent>(entity);

        // Vertices
        for (int i = 0; i < 4; i++)
        {
            glm::mat4 u_Model = transform.GetTransformMatrix();
            vertices.push_back({(uint32_t)entity + 1, glm::vec3(u_Model * glm::vec4(g_SquareData.Positions[i], 1.0f)),
                                g_SquareData.TexCoords[i],
                                TextureLibrary::GetInstance().GetTextureSlot(sprite.TextureInstance), sprite.Color});
        }

        // Indices
        for (int i = 0; i < 6; i++)
            indices.push_back(g_SquareData.Indices[i] + index * 4);

        index++;
    }

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
    auto &&view = m_Registry.view<Engine::TransformComponent, Engine::MaterialComponent>();
    for (auto &&entity : view)
    {
        auto &&[transform, materialComp] = view.get<Engine::TransformComponent, Engine::MaterialComponent>(entity);

        glm::vec4 color = glm::vec4(1.0f);
        if (auto &&colorProp = materialComp.GetProperty("Color"))
        {
            if (colorProp.GetType() == MaterialPropertyType::Vec4)
                color = colorProp.GetValueAs<glm::vec4>();
            else if (colorProp.GetType() == MaterialPropertyType::Vec3)
                color = glm::vec4(colorProp.GetValueAs<glm::vec3>(), 1.0f);
        }

        // Get Blinn-Phong material properties if available
        glm::vec4 materialData = glm::vec4(0.0f);
        if (auto &&ambientStrengthProp = materialComp.GetProperty("AmbientStrength"))
            materialData.x = ambientStrengthProp.GetValueAs<float>();
        if (auto &&diffuseStrengthProp = materialComp.GetProperty("DiffuseStrength"))
            materialData.y = diffuseStrengthProp.GetValueAs<float>();
        if (auto &&specularStrengthProp = materialComp.GetProperty("SpecularStrength"))
            materialData.z = specularStrengthProp.GetValueAs<float>();
        if (auto &&shininessProp = materialComp.GetProperty("Shininess"))
            materialData.w = shininessProp.GetValueAs<float>();

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

    m_CubesCount = index;
    VertexLibrary::GetInstance().GetVertex("Cubes")->GetVertexBuffer()->SetData(
        vertices.data(), m_CubesCount * 36 * sizeof(Vertex3D), 0);
}

void Engine::Scene::Render2D() const
{
    PROFILE_FUNCTION();

    auto &&shader = Engine::ShaderLibrary::GetInstance().GetShader("Texture");
    shader->Bind();

    // Bind textures
    auto &&spriteView = m_Registry.view<Engine::SpriteRendererComponent>();
    for (auto &&entity : spriteView)
    {
        auto &&sprite = spriteView.get<Engine::SpriteRendererComponent>(entity);
        int texID = TextureLibrary::GetInstance().GetTextureSlot(sprite.TextureInstance);
        if (texID != -1)
            sprite.TextureInstance->Bind(texID);
    }

    // Render squares
    if (m_SquaresCount > 0)
        VertexLibrary::GetInstance().GetVertex("Squares")->Render(Engine::RendererType::Triangles, m_SquaresCount * 6);
    TextureLibrary::GetInstance().ClearTextureSlots();

    shader->Unbind();
}

void Engine::Scene::RenderShadowMap() const
{
    PROFILE_FUNCTION();

    RendererCommand::CullFrontFace();
    auto &&shader = Engine::ShaderLibrary::GetInstance().GetShader("ShadowMap");
    shader->Bind();
    auto &&lightView = m_Registry.view<Engine::TransformComponent, Engine::LightComponent>();
    for (auto &&lightEntity : lightView)
    {
        auto &&[transform, light] = lightView.get<Engine::TransformComponent, Engine::LightComponent>(lightEntity);

        // Render to shadow map
        light.ShadowMap->Resize(m_ViewportWidth, m_ViewportHeight);
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
    RendererCommand::CullBackFace();
}

void Engine::Scene::Render3D() const
{
    PROFILE_FUNCTION();

    auto &&shader = ShaderLibrary::GetInstance().GetShader("BlinnPhong");
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
            light.ShadowMap->GetTexture()->Bind(lightIndex);

            lightIndex++;
        }
        shader->SetUniformInt("u_NumLights", lightIndex);
    }

    // Skybox
    auto &&view = m_Registry.view<SkyboxComponent>();
    if (!view.empty())
    {
        if (view.size() > 1)
            LOG_ENGINE_WARN("Multiple SkyboxComponents detected! Only the first one will be rendered.");

        auto &&skybox = m_Registry.get<SkyboxComponent>(view.front());
        shader->SetUniformInt("u_Skybox", lightIndex);
        skybox.GetTextureCube()->Bind(lightIndex);
    }

    TextureLibrary::GetInstance().GetTextureCube("GrassBlock")->Bind(lightIndex + 1);
    shader->SetUniformInt("u_Texture", lightIndex + 1);
    if (m_CubesCount > 0)
        VertexLibrary::GetInstance().GetVertex("Cubes")->Render(Engine::RendererType::Triangles, m_CubesCount * 36);
    TextureLibrary::GetInstance().ClearTextureSlots();

    shader->Unbind();
}

void Engine::Scene::RenderSkybox() const
{
    PROFILE_FUNCTION();

    Engine::RendererCommand::DisableDepthTest();
    auto &&shader = Engine::ShaderLibrary::GetInstance().GetShader("Skybox");
    shader->Bind();

    auto &&view = m_Registry.view<Engine::SkyboxComponent>();
    if (!view.empty())
    {
        if (view.size() > 1)
            LOG_ENGINE_WARN("Multiple SkyboxComponents detected! Only the first one will be rendered.");

        auto &&skybox = m_Registry.get<Engine::SkyboxComponent>(view.front());
        shader->SetUniformInt("u_Skybox", 0);
        skybox.GetTextureCube()->Bind(0);
        Engine::VertexLibrary::GetInstance().GetVertex("Skybox")->Render();
    }

    shader->Unbind();
    Engine::RendererCommand::EnableDepthTest();
}

void Engine::Scene::RenderColorID() const
{
    PROFILE_FUNCTION();

    m_ColorIDFrameBuffer->Resize(m_ViewportWidth, m_ViewportHeight);
    m_ColorIDFrameBuffer->Bind();
    Engine::RendererCommand::Clear();
    auto &&shader = Engine::ShaderLibrary::GetInstance().GetShader("ColorIDPicking");
    shader->Bind();

    // Render entity IDs as color IDs
    if (m_SquaresCount > 0)
        VertexLibrary::GetInstance().GetVertex("Squares")->Render(Engine::RendererType::Triangles, m_SquaresCount * 6);
    if (m_CubesCount > 0)
        VertexLibrary::GetInstance().GetVertex("Cubes")->Render(Engine::RendererType::Triangles, m_CubesCount * 36);

    shader->Unbind();
    m_ColorIDFrameBuffer->Unbind();
}
