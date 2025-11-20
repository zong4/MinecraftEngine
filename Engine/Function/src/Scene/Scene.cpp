#include "Scene.hpp"

#include "ECS/Component/Renderer/RendererComponent.hpp"
#include "Renderer/Buffer/UniformBufferLibrary.hpp"
#include "Renderer/Material/MaterialLibrary.hpp"
#include "Renderer/Shader/ShaderLibrary.hpp"
#include "Renderer/VertexArray/VAOLibrary.hpp"

Engine::Scene::~Scene()
{
    m_Registry.view<Engine::NativeScriptComponent>().each([&](auto &&entity, auto &&nsc) { nsc.DestroyScript(); });
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
        {
            transform.UpdateTransformMatrix(glm::mat4(1.0f), glm::quat(1.0f, 0.0f, 0.0f, 0.0f), &relationship);
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

void Engine::Scene::PreRender()
{
    PROFILE_FUNCTION();

    // Squares
    {
        int squareIndex = 0;
        std::vector<Vertex2D> squaresVertices;
        std::vector<unsigned int> squaresIndices;
        auto &&spriteView = m_Registry.view<Engine::TransformComponent, Engine::SpriteRendererComponent>();
        for (auto &&entity : spriteView)
        {
            auto &&[transform, sprite] =
                spriteView.get<Engine::TransformComponent, Engine::SpriteRendererComponent>(entity);

            // Vertices
            for (int i = 0; i < 4; i++)
            {
                glm::mat4 u_Model = transform.GetTransformMatrix();
                squaresVertices.push_back(
                    {(uint32_t)entity + 1, glm::vec3(u_Model * glm::vec4(g_IdentitySquareData.Positions[i], 1.0f)),
                     g_IdentitySquareData.TexCoords[i],
                     TextureLibrary::GetInstance().GetTextureSlot(sprite.TextureInstance), sprite.Color});
            }

            // Indices
            for (int i = 0; i < 6; i++)
            {
                squaresIndices.push_back(g_IdentitySquareData.Indices[i] + squareIndex * 4);
            }

            squareIndex++;
        }
        m_SquaresCount = squareIndex;
        VAOLibrary::GetInstance().GetVAO("Squares")->GetVertexBuffer().SetData(
            squaresVertices.data(), m_SquaresCount * 4 * sizeof(Vertex2D), 0);
        VAOLibrary::GetInstance().GetVAO("Squares")->GetIndexBuffer().SetData(
            squaresIndices.data(), m_SquaresCount * 6 * sizeof(unsigned int), 0);
    }

    // Cubes
    {
        int cubeIndex = 0;
        std::vector<Vertex3D> cubesVertices;
        auto &&meshView = m_Registry.view<Engine::TransformComponent, Engine::MaterialComponent>();
        for (auto &&entity : meshView)
        {
            auto &&[transform, materialComp] =
                meshView.get<Engine::TransformComponent, Engine::MaterialComponent>(entity);

            // Get material data from MaterialComponent
            glm::vec4 color = glm::vec4(1.0f);                           // Default color
            glm::vec4 materialData = glm::vec4(0.3f, 1.0f, 0.5f, 32.0f); // Default Blinn-Phong values

            if (materialComp.GetMaterial())
            {
                auto mat = materialComp.GetMaterial();
                if (mat->HasProperty("Color"))
                {
                    auto colorProp = mat->GetProperty("Color");
                    if (colorProp.GetType() == MaterialPropertyType::Vec4)
                        color = colorProp.GetVec4();
                    else if (colorProp.GetType() == MaterialPropertyType::Vec3)
                        color = glm::vec4(colorProp.GetVec3(), 1.0f);
                }
                // Get Blinn-Phong material properties if available
                if (mat->HasProperty("AmbientStrength"))
                    materialData.x = mat->GetProperty("AmbientStrength").GetFloat();
                if (mat->HasProperty("DiffuseStrength"))
                    materialData.y = mat->GetProperty("DiffuseStrength").GetFloat();
                if (mat->HasProperty("SpecularStrength"))
                    materialData.z = mat->GetProperty("SpecularStrength").GetFloat();
                if (mat->HasProperty("Shininess"))
                    materialData.w = mat->GetProperty("Shininess").GetFloat();
            }

            for (int i = 0; i < 36; ++i)
            {
                glm::mat4 u_Model = transform.GetTransformMatrix();
                cubesVertices.push_back(
                    {(uint32_t)entity + 1, glm::vec3(u_Model * glm::vec4(g_IdentityCubeData.Positions[i], 1.0f)),
                     glm::normalize(glm::transpose(glm::inverse(glm::mat3(u_Model))) * g_IdentityCubeData.Normals[i]),
                     g_IdentityCubeData.Positions[i], color, materialData});
            }
            cubeIndex++;
        }
        m_CubesCount = cubeIndex;
        VAOLibrary::GetInstance().GetVAO("Cubes")->GetVertexBuffer().SetData(cubesVertices.data(),
                                                                             m_CubesCount * 36 * sizeof(Vertex3D), 0);
    }
}

// todo: only render for the first light for now
void Engine::Scene::RenderShadowMap() const
{
    PROFILE_FUNCTION();

    RendererCommand::CullFrontFace();
    auto &&shader = Engine::ShaderLibrary::GetInstance().GetShader("ShadowMap");
    shader->Bind();
    auto &&lightView = m_Registry.view<Engine::TransformComponent, Engine::LightComponent>();
    for (auto &&lightEntity : lightView)
    {
        m_ShadowMap->Bind();
        RendererCommand::ClearDepthBuffer();

        auto &&[transform, light] = lightView.get<Engine::TransformComponent, Engine::LightComponent>(lightEntity);
        shader->SetUniformMat4("u_LightView", glm::inverse(transform.GetTransformMatrix()));
        shader->SetUniformMat4("u_LightProjection", glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 1.0f, 20.0f));

        if (m_CubesCount > 0)
            VAOLibrary::GetInstance().GetVAO("Cubes")->Render(Engine::RendererType::Triangles, m_CubesCount * 36);

        m_ShadowMap->Unbind();

        break;
    }
    shader->Unbind();
    RendererCommand::CullBackFace();
}

void Engine::Scene::Render(const Entity &camera) const
{
    PROFILE_FUNCTION();

    // Clear buffers
    {
        RendererCommand::SetClearColor(camera.GetComponent<CameraComponent>()->BackgroundColor);
        RendererCommand::Clear();
    }

    // Update camera
    {
        auto &&transform = camera.GetComponent<TransformComponent>();
        auto &&cameraComp = camera.GetComponent<CameraComponent>();
        if (!transform || !cameraComp)
            return;

        cameraComp->UpdateProjectionMatrix();
        UniformBufferLibrary::GetInstance().UpdateUniformBuffer(
            "UniformBuffer0",
            {
                {glm::value_ptr(glm::inverse(transform->GetTransformMatrix())), sizeof(glm::mat4), 0},
                {glm::value_ptr(cameraComp->GetProjectionMatrix()), sizeof(glm::mat4), sizeof(glm::mat4)},
                {glm::value_ptr(transform->Position), sizeof(glm::vec3), sizeof(glm::mat4) + sizeof(glm::mat4)},
            });
    }

    // // Render
    // {
    //     Render2D();
    //     Render3D();
    //     RenderSkybox();
    // }
}

void Engine::Scene::RenderColorID() const
{
    PROFILE_FUNCTION();

    // Clear buffers
    Engine::RendererCommand::Clear();

    // Render
    {
        auto &&shader = Engine::ShaderLibrary::GetInstance().GetShader("ColorIDPicking");
        shader->Bind();

        if (m_SquaresCount > 0)
            VAOLibrary::GetInstance().GetVAO("Squares")->Render(Engine::RendererType::Triangles, m_SquaresCount * 6);
        if (m_CubesCount > 0)
            VAOLibrary::GetInstance().GetVAO("Cubes")->Render(Engine::RendererType::Triangles, m_CubesCount * 36);

        shader->Unbind();
    }
}

void Engine::Scene::Resize(float width, float height)
{
    PROFILE_FUNCTION();

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
    entity.AddComponent<TagComponent>(name);
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

// todo: check
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
        VAOLibrary::GetInstance().GetVAO("Squares")->Render(Engine::RendererType::Triangles, m_SquaresCount * 6);
    TextureLibrary::GetInstance().ClearTextureSlots();

    shader->Unbind();
}

// todo: only render for the first light for now
void Engine::Scene::Render3D() const
{
    PROFILE_FUNCTION();

    auto &&shader = ShaderLibrary::GetInstance().GetShader("BlinnPhong");
    shader->Bind();

    // Light
    int lightIndex = 0;
    auto &&lightView = m_Registry.view<Engine::TransformComponent, Engine::LightComponent>();
    for (auto &&entity : lightView)
    {
        auto &&[transform, light] = lightView.get<Engine::TransformComponent, Engine::LightComponent>(entity);

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
    if (m_CubesCount > 0)
        VAOLibrary::GetInstance().GetVAO("Cubes")->Render(Engine::RendererType::Triangles, m_CubesCount * 36);
    TextureLibrary::GetInstance().ClearTextureSlots();
    shader->Unbind();
}

// todo: default skybox if none exists
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
        Engine::VAOLibrary::GetInstance().GetVAO("Skybox")->Render();
    }

    shader->Unbind();
    Engine::RendererCommand::EnableDepthTest();
}
