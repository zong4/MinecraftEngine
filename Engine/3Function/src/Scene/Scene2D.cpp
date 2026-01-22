#include "Scene2D.hpp"

#include "../Renderer/Library/UniformLibrary.hpp"
#include "../Renderer/Library/VertexLibrary.hpp"

void Engine::Scene2D::Render(const Entity &camera)
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

    // Upload data
    UploadSquaresData();

    // Render scene
    Render2D(camera);
    RenderColorID();
}

void Engine::Scene2D::RenderColorID() const
{
    PROFILE_FUNCTION();

    m_ColorIDFrameBuffer->Bind();
    Engine::RendererCommand::Clear();
    auto &&shader = Engine::ShadersManager::GetInstance().GetShader("ColorIDPicking");
    shader->Bind();

    // Render entity IDs as color IDs
    if (m_SquaresCount > 0)
        VertexLibrary::GetInstance().GetVertex("Squares")->Render(Engine::RendererType::Triangles, m_SquaresCount * 6);

    shader->Unbind();
    m_ColorIDFrameBuffer->Unbind();
}

void Engine::Scene2D::UploadSquaresData()
{
    PROFILE_FUNCTION();

    int index = 0;
    std::vector<Vertex2D> vertices;
    std::vector<unsigned int> indices;
    auto &&view =
        m_Registry.view<Engine::TransformComponent, Engine::SpriteRendererComponent, Engine::MaterialComponent>();
    for (auto &&entity : view)
    {
        auto &&[transform, sprite, material] =
            view.get<Engine::TransformComponent, Engine::SpriteRendererComponent, Engine::MaterialComponent>(entity);

        // Vertices
        for (int i = 0; i < 4; i++)
        {
            glm::mat4 u_Model = transform.GetTransformMatrix();
            vertices.push_back({(uint32_t)entity + 1, glm::vec3(u_Model * glm::vec4(g_SquareData.Positions[i], 1.0f)),
                                g_SquareData.TexCoords[i],
                                TexturesManager::GetInstance().GetTextureSlot(
                                    material.GetProperty("Texture").GetValueAs<std::shared_ptr<Texture>>()),
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

void Engine::Scene2D::Render2D(const Entity &camera) const
{
    PROFILE_FUNCTION();

    // Clear buffers
    RendererCommand::SetClearColor(camera.GetComponent<CameraComponent>()->BackgroundColor);
    RendererCommand::Clear();

    // Shader is same for 2d sprites
    auto &&shader = Engine::ShadersManager::GetInstance().GetShader("Texture");
    shader->Bind();

    // Bind textures
    auto &&view = m_Registry.view<Engine::MaterialComponent>();
    for (auto &&entity : view)
    {
        auto &&material = view.get<Engine::MaterialComponent>(entity);
        auto &&texture = material.GetProperty("Texture").GetValueAs<std::shared_ptr<Texture>>();
        int texID = TexturesManager::GetInstance().GetTextureSlot(texture);
        if (texID != -1)
            texture->Active(texID);
    }

    // Render squares
    if (m_SquaresCount > 0)
        VertexLibrary::GetInstance().GetVertex("Squares")->Render(Engine::RendererType::Triangles, m_SquaresCount * 6);
    TexturesManager::GetInstance().ClearTextureSlots();

    shader->Unbind();
}
