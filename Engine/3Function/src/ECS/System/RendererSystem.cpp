#include "RendererSystem.hpp"

#include "../../Renderer/Library/UniformLibrary.hpp"
#include "../../Renderer/Library/VertexLibrary.hpp"
#include "../../ResourcesManager/ShadersManager.hpp"
#include "../../ResourcesManager/TexturesManager.hpp"

Engine::RendererSystem::RendererSystem() : m_SkyboxTexture(TexturesManager::GetInstance().GetTextureCube("Default")) {}

void Engine::RendererSystem::Resize(entt::registry &registry, int width, int height)
{
    PROFILE_FUNCTION();

    // Resize all cameras
    auto &&cameraView = registry.view<CameraComponent>();
    for (auto &&entity : cameraView)
    {
        auto &&camera = cameraView.get<CameraComponent>(entity);
        camera.Resize(width, height);
    }

    // Resize all light shadow maps
    auto &&lightView = registry.view<LightComponent>();
    for (auto &&entity : lightView)
    {
        auto &&light = lightView.get<LightComponent>(entity);
        light.ShadowMap->Resize(width, height);
    }

    // Resize color ID framebuffer
    m_ColorIDFrameBuffer->Resize(width, height);
}

void Engine::RendererSystem::Render(entt::registry &registry)
{
    PROFILE_FUNCTION();

    // 2D
    UploadSquares(registry);
    Render2D(registry);

    // 3D
    UploadCubes(registry);
    RenderShadowMap(registry);
    Render3D(registry);

    // Common
    RenderColorID();
}

void Engine::RendererSystem::RenderSkybox() const
{
    PROFILE_FUNCTION();

    RendererCommand::SetFaceCulling(CullingFace::Front);
    RendererCommand::SetDepthTestFunction(DepthTestFunction::LessEqual);
    RendererCommand::SetDepthWrite(false);
    auto &&shader = Engine::ShadersManager::GetInstance().GetShader("Skybox");
    shader->Bind();

    shader->SetUniformInt("u_Skybox", 0);
    m_SkyboxTexture->Active(0);
    Engine::VertexLibrary::GetInstance().GetVertex("Cube")->Render();

    shader->Unbind();
    RendererCommand::SetDepthWrite(true);
    RendererCommand::SetDepthTestFunction(DepthTestFunction::Less);
    RendererCommand::SetFaceCulling(CullingFace::Back);
}

void Engine::RendererSystem::UploadSquares(entt::registry &registry)
{
    PROFILE_FUNCTION();

    int index = 0;
    std::vector<Vertex2D> vertices;
    std::vector<unsigned int> indices;
    auto &&view =
        registry.view<Engine::TransformComponent, Engine::SpriteRendererComponent, Engine::MaterialComponent>();
    for (auto &&entity : view)
    {
        auto &&[transform, spriteRenderer, material] =
            view.get<Engine::TransformComponent, Engine::SpriteRendererComponent, Engine::MaterialComponent>(entity);

        // Vertices
        for (int i = 0; i < 4; i++)
        {
            glm::mat4 u_Model = transform.GetTransformMatrix();
            vertices.push_back({(uint32_t)entity + 1, glm::vec3(u_Model * glm::vec4(g_SquareData.Positions[i], 1.0f)),
                                material.GetProperty("Color").GetValueAs<glm::vec4>(), g_SquareData.TexCoords[i],
                                TexturesManager::GetInstance().GetTextureSlot(
                                    material.GetProperty("Texture").GetValueAs<std::shared_ptr<Texture2D>>())});
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

void Engine::RendererSystem::UploadCubes(entt::registry &registry)
{
    PROFILE_FUNCTION();

    int index = 0;
    std::vector<Vertex3D> vertices;
    auto &&view = registry.view<Engine::TransformComponent, Engine::MeshRendererComponent, Engine::MaterialComponent>();
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
                 glm::normalize(glm::transpose(glm::inverse(glm::mat3(u_Model))) * g_CubeData.Normals[i]), materialData,
                 color, g_CubeData.Positions[i], 0});
        }
        index++;
    }

    // Update count and buffer data
    m_CubesCount = index;
    VertexLibrary::GetInstance().GetVertex("Cubes")->GetVertexBuffer()->SetData(
        vertices.data(), m_CubesCount * 36 * sizeof(Vertex3D), 0);
}

void Engine::RendererSystem::Render2D(entt::registry &registry) const
{
    PROFILE_FUNCTION();

    // Shader is same for 2d sprites
    auto &&shader = Engine::ShadersManager::GetInstance().GetShader("Texture2D");
    shader->Bind();

    // Textures
    auto &&view = registry.view<Engine::SpriteRendererComponent, Engine::MaterialComponent>();
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

void Engine::RendererSystem::RenderShadowMap(entt::registry &registry) const
{
    RendererCommand::SetFaceCulling(CullingFace::Front);
    auto &&shader = Engine::ShadersManager::GetInstance().GetShader("ShadowMap");
    shader->Bind();
    auto &&view = registry.view<Engine::TransformComponent, Engine::LightComponent>();
    for (auto &&entity : view)
    {
        auto &&[transform, light] = view.get<Engine::TransformComponent, Engine::LightComponent>(entity);

        // Render to shadow map
        light.ShadowMap->Bind();
        RendererCommand::ClearDepthBuffer();
        {
            shader->SetUniformMat4("u_LightView", glm::inverse(transform.GetTransformMatrix()));
            shader->SetUniformMat4("u_LightProjection", glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 1.0f,
                                                                   20.0f)); // todo: calculate camera view

            // Render
            if (m_CubesCount > 0)
                VertexLibrary::GetInstance().GetVertex("Cubes")->Render(Engine::RendererType::Triangles,
                                                                        m_CubesCount * 36);
        }
        light.ShadowMap->Unbind();
    }
    shader->Unbind();
    RendererCommand::SetFaceCulling(CullingFace::Back);
}

void Engine::RendererSystem::Render3D(entt::registry &registry) const
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
        auto &&lightView = registry.view<Engine::TransformComponent, Engine::LightComponent>();
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
    shader->SetUniformInt("u_TextureGrass", lightIndex + 1);
    TexturesManager::GetInstance().GetTextureCube("DefaultSkybox")->Active(lightIndex + 2);
    shader->SetUniformInt("u_TextureWhite", lightIndex + 2);

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

void Engine::RendererSystem::RenderColorID() const
{
    PROFILE_FUNCTION();

    m_ColorIDFrameBuffer->Bind();
    Engine::RendererCommand::Clear();
    auto &&shader = Engine::ShadersManager::GetInstance().GetShader("ColorID");
    shader->Bind();

    // Render entity IDs as color IDs
    if (m_SquaresCount > 0)
        VertexLibrary::GetInstance().GetVertex("Squares")->Render(Engine::RendererType::Triangles, m_SquaresCount * 6);
    if (m_CubesCount > 0)
        VertexLibrary::GetInstance().GetVertex("Cubes")->Render(Engine::RendererType::Triangles, m_CubesCount * 36);

    shader->Unbind();
    m_ColorIDFrameBuffer->Unbind();
}