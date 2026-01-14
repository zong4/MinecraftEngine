#include "Scene2D.hpp"

#include "../Renderers/Librarys/ShaderLibrary.hpp"
#include "../Renderers/Librarys/UniformLibrary.hpp"
#include "../Renderers/Librarys/VertexLibrary.hpp"

void Engine::Scene2D::Render(const Entity &camera)
{
    PROFILE_FUNCTION();

    UploadSquaresData();

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

    Render2D(camera);
    RenderColorID();
}

void Engine::Scene2D::Resize(int width, int height)
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

void Engine::Scene2D::RenderColorID() const
{
    PROFILE_FUNCTION();

    m_ColorIDFrameBuffer->Bind();
    Engine::RendererCommand::Clear();
    auto &&shader = Engine::ShaderLibrary::GetInstance().GetShader("ColorIDPicking");
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
    auto &&view = m_Registry.view<Engine::TransformComponent, Engine::SpriteRendererComponent>();
    for (auto &&entity : view)
    {
        auto &&[transform, sprite] = view.get<Engine::TransformComponent, Engine::SpriteRendererComponent>(entity);

        // Vertices
        for (int i = 0; i < 4; i++)
        {
            glm::mat4 u_Model = transform.GetTransformMatrix();
            vertices.push_back({(uint32_t)entity + 1, glm::vec3(u_Model * glm::vec4(g_SquareData.Positions[i], 1.0f)),
                                g_SquareData.TexCoords[i], TextureLibrary::GetInstance().GetTextureSlot(sprite.Texture),
                                sprite.Color});
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

    auto &&shader = Engine::ShaderLibrary::GetInstance().GetShader("Texture");
    shader->Bind();

    // Bind textures
    auto &&spriteView = m_Registry.view<Engine::SpriteRendererComponent>();
    for (auto &&entity : spriteView)
    {
        auto &&sprite = spriteView.get<Engine::SpriteRendererComponent>(entity);
        int texID = TextureLibrary::GetInstance().GetTextureSlot(sprite.Texture);
        if (texID != -1)
            sprite.Texture->Active(texID);
    }

    // Render squares
    if (m_SquaresCount > 0)
        VertexLibrary::GetInstance().GetVertex("Squares")->Render(Engine::RendererType::Triangles, m_SquaresCount * 6);
    TextureLibrary::GetInstance().ClearTextureSlots();

    shader->Unbind();
}
