#include "Scene.hpp"

#include <imgui.h>

MCEngine::Scene::~Scene()
{
    ENGINE_PROFILE_FUNCTION();

    m_Registry.view<MCEngine::NativeScriptComponent>().each([&](auto &&entity, auto &&nsc) { nsc.DestroyScript(); });
}

void MCEngine::Scene::Update(float deltaTime)
{
    ENGINE_PROFILE_FUNCTION();

    // Update all transform matrices
    auto &&view = m_Registry.view<TransformComponent, RelationshipComponent>();
    for (auto &&entity : view)
    {
        auto &&[transform, relationship] = view.get<TransformComponent, RelationshipComponent>(entity);
        if (!relationship.Parent)
        {
            transform.UpdateTransformMatrix(glm::mat4(1.0f), glm::quat(1.0f, 0.0f, 0.0f, 0.0f), relationship);
        }
    }

    // Update all scripts
    m_Registry.view<MCEngine::NativeScriptComponent>().each([&](auto &&entity, auto &&nsc) {
        if (!nsc.Instance)
        {
            nsc.Instance = nsc.InstantiateScript();
            nsc.Instance->OnStart();
        }
        nsc.Instance->OnUpdate(deltaTime);
    });
}

void MCEngine::Scene::PreRender()
{
    ENGINE_PROFILE_FUNCTION();

    int squareIndex = 0;
    std::vector<Vertex2D> squaresVertices;
    std::vector<unsigned int> squaresIndices;
    auto &&spriteView = m_Registry.view<MCEngine::TransformComponent, MCEngine::SpriteRendererComponent>();
    for (auto &&entity : spriteView)
    {
        auto &&[transform, sprite] =
            spriteView.get<MCEngine::TransformComponent, MCEngine::SpriteRendererComponent>(entity);
        for (int i = 0; i < 4; i++)
        {
            glm::mat4 u_Model = transform.GetTransformMatrix();
            squaresVertices.push_back(
                {(uint32_t)entity + 1, glm::vec3(u_Model * glm::vec4(g_IdentitySquareData.Positions[i], 1.0f)),
                 g_IdentitySquareData.TexCoords[i],
                 TextureLibrary::GetInstance().GetTextureSlot(sprite.TextureInstance), sprite.Color});
        }
        for (int i = 0; i < 6; i++)
        {
            squaresIndices.push_back(g_IdentitySquareData.Indices[i] + squareIndex * 4);
        }
        squareIndex++;
    }
    m_SquaresCount = squareIndex;
    VAOLibrary::GetInstance().GetVAO("Squares")->GetVertexBuffer().SetData(squaresVertices.data(),
                                                                           m_SquaresCount * 4 * sizeof(Vertex2D), 0);
    VAOLibrary::GetInstance().GetVAO("Squares")->GetIndexBuffer().SetData(squaresIndices.data(),
                                                                          m_SquaresCount * 6 * sizeof(unsigned int), 0);

    int cubeIndex = 0;
    std::vector<Vertex3D> cubesVertices;
    auto &&meshView = m_Registry.view<MCEngine::TransformComponent, MCEngine::MeshRendererComponent>();
    for (auto &&entity : meshView)
    {
        auto &&[transform, mesh] = meshView.get<MCEngine::TransformComponent, MCEngine::MeshRendererComponent>(entity);
        for (int i = 0; i < 36; ++i)
        {
            glm::mat4 u_Model = transform.GetTransformMatrix();
            cubesVertices.push_back(
                {(uint32_t)entity + 1, glm::vec3(u_Model * glm::vec4(g_IdentityCubeData.Positions[i], 1.0f)),
                 glm::normalize(glm::transpose(glm::inverse(glm::mat3(u_Model))) * g_IdentityCubeData.Normals[i]),
                 g_IdentityCubeData.Positions[i], mesh.MaterialInstance.Color,
                 glm::vec4(mesh.MaterialInstance.AmbientStrength, mesh.MaterialInstance.DiffuseStrength,
                           mesh.MaterialInstance.SpecularStrength, mesh.MaterialInstance.Shininess)});
        }
        cubeIndex++;
    }
    m_CubesCount = cubeIndex;
    VAOLibrary::GetInstance().GetVAO("Cubes")->GetVertexBuffer().SetData(cubesVertices.data(),
                                                                         m_CubesCount * 36 * sizeof(Vertex3D), 0);
}

void MCEngine::Scene::RenderShadowMap() const
{
    ENGINE_PROFILE_FUNCTION();

    RendererCommand::CullFrontFace();
    auto &&shader = MCEngine::ShaderLibrary::GetInstance().GetShader("ShadowMap");
    shader->Bind();
    auto &&lightView = m_Registry.view<MCEngine::TransformComponent, MCEngine::LightComponent>();
    for (auto &&lightEntity : lightView)
    {
        m_ShadowMap->Bind();
        RendererCommand::ClearDepthBuffer();

        auto &&[transform, light] = lightView.get<MCEngine::TransformComponent, MCEngine::LightComponent>(lightEntity);
        shader->SetUniformMat4("u_LightView", glm::inverse(transform.GetTransformMatrix()));
        shader->SetUniformMat4("u_LightProjection", glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 1.0f, 20.0f));

        VAOLibrary::GetInstance().GetVAO("Cubes")->Render(MCEngine::RendererType::Triangles, m_CubesCount * 36);

        m_ShadowMap->Unbind();

        // todo
        break;
    }
    shader->Unbind();
    RendererCommand::CullBackFace();
}

void MCEngine::Scene::Render(const Entity &camera) const
{
    ENGINE_PROFILE_FUNCTION();

    // Update camera
    {
        if (!camera || !camera.HasComponent<CameraComponent>() || !camera.HasComponent<TransformComponent>())
            return;
        camera.GetComponent<CameraComponent>().UpdateProjectionMatrix();
        UniformBufferLibrary::GetInstance().UpdateUniformBuffer(
            "UniformBuffer0",
            {
                {glm::value_ptr(glm::inverse(camera.GetComponent<TransformComponent>().GetTransformMatrix())),
                 sizeof(glm::mat4), 0},
                {glm::value_ptr(camera.GetComponent<CameraComponent>().GetProjectionMatrix()), sizeof(glm::mat4),
                 sizeof(glm::mat4)},
                {glm::value_ptr(camera.GetComponent<TransformComponent>().Position), sizeof(glm::vec3),
                 sizeof(glm::mat4) + sizeof(glm::mat4)},
            });
    }

    // Render
    Render2D();
    Render3D();
    RenderSkybox();
}

void MCEngine::Scene::RenderColorID() const
{
    ENGINE_PROFILE_FUNCTION();

    auto &&shader = MCEngine::ShaderLibrary::GetInstance().GetShader("ColorIDPicking");
    shader->Bind();

    VAOLibrary::GetInstance().GetVAO("Squares")->Render(MCEngine::RendererType::Triangles, m_SquaresCount * 6);
    VAOLibrary::GetInstance().GetVAO("Cubes")->Render(MCEngine::RendererType::Triangles, m_CubesCount * 36);

    shader->Unbind();
}

void MCEngine::Scene::Resize(float width, float height)
{
    ENGINE_PROFILE_FUNCTION();

    auto &&view = m_Registry.view<CameraComponent>();
    for (auto &&entity : view)
    {
        auto &&camera = view.get<CameraComponent>(entity);
        camera.Resize(width, height);
    }
}

void MCEngine::Scene::DeleteEntity(const Entity &entity)
{
    ENGINE_PROFILE_FUNCTION();

    if (!entity)
        return;

    // Remove from parent's children list and Recursively delete children
    if (entity.HasComponent<RelationshipComponent>())
    {
        auto &&relationship = entity.GetComponent<RelationshipComponent>();
        if (relationship.Parent && relationship.Parent.HasComponent<RelationshipComponent>())
        {
            auto &&parentRelationship = relationship.Parent.GetComponent<RelationshipComponent>();
            parentRelationship.RemoveChild(entity);
        }

        auto children = relationship.GetChildren();
        for (auto &&child : children)
        {
            DeleteEntity(child);
        }
    }

    // Call OnDestroy for NativeScriptComponent
    if (entity.HasComponent<NativeScriptComponent>())
    {
        entity.GetComponent<NativeScriptComponent>().DestroyScript();
    }

    m_Registry.destroy(entity.GetHandle());
}

MCEngine::Entity MCEngine::Scene::AddEmptyEntity(const std::string &name, const TransformComponent &transform)
{
    ENGINE_PROFILE_FUNCTION();

    Entity entity{m_Registry.create(), &m_Registry};
    entity.AddComponent<TagComponent>(name);
    entity.AddComponent<TransformComponent>(transform);
    entity.AddComponent<RelationshipComponent>();
    return entity;
}

MCEngine::Entity MCEngine::Scene::Add2DObject(const std::string &name, const TransformComponent &transform,
                                              const SpriteRendererComponent &spriteRenderer)
{
    Entity entity = AddEmptyEntity(name, transform);
    entity.AddComponent<SpriteRendererComponent>(spriteRenderer);
    return entity;
}

MCEngine::Entity MCEngine::Scene::Add3DObject(const std::string &name, const TransformComponent &transform,
                                              const MeshRendererComponent &meshRenderer)
{
    Entity entity = AddEmptyEntity(name, transform);
    entity.AddComponent<MeshRendererComponent>(meshRenderer);
    return entity;
}

MCEngine::Entity MCEngine::Scene::AddCamera(const std::string &name, const TransformComponent &transform,
                                            const CameraComponent &cameraComponent)
{
    Entity entity = AddEmptyEntity(name, transform);
    entity.AddComponent<CameraComponent>(cameraComponent);
    return entity;
}

MCEngine::Entity MCEngine::Scene::AddLight(const std::string &name, const TransformComponent &transform,
                                           const LightComponent &lightComponent)
{
    Entity entity = AddEmptyEntity(name, transform);
    entity.AddComponent<LightComponent>(lightComponent);
    return entity;
}

MCEngine::Entity MCEngine::Scene::AddSkybox(const std::string &name, const SkyboxComponent &skyboxComponent)
{
    Entity entity = AddEmptyEntity(name);
    entity.AddComponent<SkyboxComponent>(skyboxComponent);
    return entity;
}

void MCEngine::Scene::Render2D() const
{
    ENGINE_PROFILE_FUNCTION();

    auto &&shader = MCEngine::ShaderLibrary::GetInstance().GetShader("Texture");
    shader->Bind();

    auto &&spriteView = m_Registry.view<MCEngine::SpriteRendererComponent>();
    for (auto &&entity : spriteView)
    {
        auto &&sprite = spriteView.get<MCEngine::SpriteRendererComponent>(entity);
        int texID = TextureLibrary::GetInstance().GetTextureSlot(sprite.TextureInstance);
        if (texID != -1)
            sprite.TextureInstance->Bind(texID);
    }
    VAOLibrary::GetInstance().GetVAO("Squares")->Render(MCEngine::RendererType::Triangles, m_SquaresCount * 6);
    TextureLibrary::GetInstance().ClearTextureSlots();
    shader->Unbind();
}

void MCEngine::Scene::Render3D() const
{
    ENGINE_PROFILE_FUNCTION();

    auto &&shader = ShaderLibrary::GetInstance().GetShader("BlinnPhong");
    shader->Bind();

    // Light
    int lightIndex = 0;
    auto &&lightView = m_Registry.view<MCEngine::TransformComponent, MCEngine::LightComponent>();
    for (auto &&entity : lightView)
    {
        auto &&[transform, light] = lightView.get<MCEngine::TransformComponent, MCEngine::LightComponent>(entity);

        // Light
        shader->SetUniformInt("u_Light[" + std::to_string(lightIndex) + "].Type", static_cast<int>(light.GetType()));
        shader->SetUniformVec3("u_Light[" + std::to_string(lightIndex) + "].Position", transform.Position);
        shader->SetUniformVec3("u_Light[" + std::to_string(lightIndex) + "].Color", light.Color * light.Intensity);
        shader->SetUniformFloat("u_Light[" + std::to_string(lightIndex) + "].Constant", light.Constant);
        shader->SetUniformFloat("u_Light[" + std::to_string(lightIndex) + "].Linear", light.Linear);
        shader->SetUniformFloat("u_Light[" + std::to_string(lightIndex) + "].Quadratic", light.Quadratic);
        shader->SetUniformFloat("u_Light[" + std::to_string(lightIndex) + "].CutOff",
                                glm::cos(glm::radians(light.InnerAngle)));
        shader->SetUniformFloat("u_Light[" + std::to_string(lightIndex) + "].OuterCutOff",
                                glm::cos(glm::radians(light.OuterAngle)));

        // Shadow
        shader->SetUniformMat4("u_LightView[" + std::to_string(lightIndex) + "]",
                               glm::inverse(transform.GetTransformMatrix()));
        shader->SetUniformMat4("u_LightProjection[" + std::to_string(lightIndex) + "]",
                               glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 1.0f, 20.0f));
        shader->SetUniformInt("u_ShadowMap[" + std::to_string(lightIndex) + "]", lightIndex);
        m_ShadowMap->GetTexture()->Bind(lightIndex);

        lightIndex++;

        // todo
        break;
    }
    shader->SetUniformInt("u_NumLights", lightIndex);

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
    VAOLibrary::GetInstance().GetVAO("Cubes")->Render(MCEngine::RendererType::Triangles, m_CubesCount * 36);
    TextureLibrary::GetInstance().ClearTextureSlots();
    shader->Unbind();
}

void MCEngine::Scene::RenderSkybox() const
{
    ENGINE_PROFILE_FUNCTION();

    MCEngine::RendererCommand::DisableDepthTest();
    auto &&shader = MCEngine::ShaderLibrary::GetInstance().GetShader("Skybox");
    shader->Bind();

    auto &&view = m_Registry.view<MCEngine::SkyboxComponent>();
    if (!view.empty())
    {
        if (view.size() > 1)
            LOG_ENGINE_WARN("Multiple SkyboxComponents detected! Only the first one will be rendered.");

        auto &&skybox = m_Registry.get<MCEngine::SkyboxComponent>(view.front());
        shader->SetUniformInt("u_Skybox", 0);
        skybox.GetTextureCube()->Bind(0);
        MCEngine::VAOLibrary::GetInstance().GetVAO("Skybox")->Render();
    }

    shader->Unbind();
    MCEngine::RendererCommand::EnableDepthTest();
}
