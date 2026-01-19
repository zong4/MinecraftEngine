#include "Scene3D.hpp"

#include "../Physic/Ray/RayTracing.hpp"
#include "../Renderer/Librarys/VertexLibrary.hpp"
#include "../Renderer/Material/MaterialLibrary.hpp"

Engine::Scene3D::Scene3D(const std::string &name) : Scene(name) {}

void Engine::Scene3D::Render(const Entity &camera)
{
    UploadCubesData();

    // Render scene
    Scene::Render(camera);
    RenderShadowMap();
    Render3D(camera);
    RenderSkybox();
    RenderColorID();

    // // Ray tracing
    // if (!m_RayTracingRunning.exchange(true))
    // {
    //     std::thread([this, camera] {
    //         std::vector<Entity> entities;
    //         auto &&registry = this->GetRegistry();
    //         auto &&view = registry.view<TransformComponent, MeshRendererComponent>();
    //         for (auto &&entity : view)
    //         {
    //             entities.push_back(Entity{entity, &registry});
    //         }
    //         std::vector<glm::vec4> tempBuffer;
    //         Engine::RayTracing::RenderScene(camera, entities, 100, 1, tempBuffer);
    //         m_RayTracingRunning = false;
    //     }).detach();
    // }
}

void Engine::Scene3D::Resize(int width, int height)
{
    Scene::Resize(width, height);

    // Resize shadow map framebuffers for lights
    auto &&lightView = m_Registry.view<LightComponent>();
    for (auto &&entity : lightView)
    {
        auto &&lightComp = lightView.get<LightComponent>(entity);
        lightComp.ShadowMap->Resize(width, height);
    }
}

void Engine::Scene3D::RenderColorID() const
{
    PROFILE_FUNCTION();

    m_ColorIDFrameBuffer->Bind();
    RendererCommand::Clear();
    auto &&shader = Engine::ShaderLibrary::GetInstance().GetShader("ColorIDPicking");
    shader->Bind();

    // Render entity IDs as color IDs
    if (m_CubesCount > 0)
        VertexLibrary::GetInstance().GetVertex("Cubes")->Render(Engine::RendererType::Triangles, m_CubesCount * 36);

    shader->Unbind();
    m_ColorIDFrameBuffer->Unbind();
}

void Engine::Scene3D::UploadCubesData()
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

    // Update count and buffer data
    m_CubesCount = index;
    VertexLibrary::GetInstance().GetVertex("Cubes")->GetVertexBuffer()->SetData(
        vertices.data(), m_CubesCount * 36 * sizeof(Vertex3D), 0);
}

void Engine::Scene3D::RenderShadowMap() const
{
    PROFILE_FUNCTION();

    RendererCommand::SetFaceCulling(CullingFace::Front);
    auto &&shader = Engine::ShaderLibrary::GetInstance().GetShader("ShadowMap");
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

void Engine::Scene3D::Render3D(const Entity &camera) const
{
    PROFILE_FUNCTION();

    // Clear buffers
    RendererCommand::SetClearColor(camera.GetComponent<CameraComponent>()->BackgroundColor);
    RendererCommand::Clear();

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
            light.ShadowMap->GetTexture()->Active(lightIndex);

            lightIndex++;
        }
        shader->SetUniformInt("u_NumLights", lightIndex);
    }

    // Skybox
    shader->SetUniformInt("u_Skybox", lightIndex);
    m_SkyboxTexture->Active(lightIndex);

    TextureLibrary::GetInstance().GetTextureCube("GrassBlock")->Active(lightIndex + 1);
    shader->SetUniformInt("u_Texture", lightIndex + 1);
    if (m_CubesCount > 0)
        VertexLibrary::GetInstance().GetVertex("Cubes")->Render(Engine::RendererType::Triangles, m_CubesCount * 36);
    TextureLibrary::GetInstance().ClearTextureSlots();

    shader->Unbind();
}

void Engine::Scene3D::RenderSkybox() const
{
    PROFILE_FUNCTION();

    RendererCommand::SetFaceCulling(CullingFace::Front);
    RendererCommand::SetDepthTestFunction(DepthTestFunction::LessEqual);
    RendererCommand::SetDepthWrite(false);
    auto &&shader = Engine::ShaderLibrary::GetInstance().GetShader("Skybox");
    shader->Bind();

    shader->SetUniformInt("u_Skybox", 0);
    m_SkyboxTexture->Active(0);
    Engine::VertexLibrary::GetInstance().GetVertex("Cube")->Render();

    shader->Unbind();
    RendererCommand::SetDepthWrite(true);
    RendererCommand::SetDepthTestFunction(DepthTestFunction::Less);
    RendererCommand::SetFaceCulling(CullingFace::Back);
}
