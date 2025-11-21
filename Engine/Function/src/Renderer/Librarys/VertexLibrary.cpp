#include "VertexLibrary.hpp"

namespace Engine
{

struct SkyboxCubeData
{
    float vertices[108] = {
        // clang-format off
        // positions          
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
        // clang-format on
    };
};
inline const SkyboxCubeData g_SkyboxCubeData;

} // namespace Engine

Engine::VertexLibrary &Engine::VertexLibrary::GetInstance()
{
    static VertexLibrary instance;
    return instance;
}

std::string Engine::VertexLibrary::GetName(const std::shared_ptr<VertexArray> &vao) const
{
    for (const auto &[name, ptr] : m_VertexMap)
    {
        if (ptr == vao)
            return name;
    }
    LOG_ENGINE_ERROR("VAO not found in library");
    return "";
}

std::shared_ptr<Engine::VertexArray> Engine::VertexLibrary::GetVertex(const std::string &name)
{
    if (!Exists(name))
    {
        LOG_ENGINE_ERROR("VAO not found: " + name);
        return nullptr;
    }
    return m_VertexMap[name];
}

void Engine::VertexLibrary::AddVertex(const std::string &name, const std::shared_ptr<VertexArray> &vao)
{
    if (Exists(name))
    {
        LOG_ENGINE_WARN("VAO already exists: " + name);
        return;
    }
    m_VertexMap[name] = vao;
    LOG_ENGINE_TRACE("VAO added: " + name);
}

Engine::VertexLibrary::VertexLibrary()
{
    PROFILE_FUNCTION();

    static int MaxSquaresNumber = 10000;
    auto &&squaresVAO = std::make_shared<VertexArray>(
        VertexBuffer(MaxSquaresNumber * sizeof(Vertex2D) * 4),
        std::vector<VertexAttribute>{
            {0, 1, VertexAttributeType::UnsignedInt, 0, sizeof(Vertex2D), (const void *)(0 * sizeof(float))},
            {1, 3, VertexAttributeType::Float, 0, sizeof(Vertex2D), (const void *)(1 * sizeof(float))},
            {2, 2, VertexAttributeType::Float, 0, sizeof(Vertex2D), (const void *)(4 * sizeof(float))},
            {3, 1, VertexAttributeType::Int, 0, sizeof(Vertex2D), (const void *)(6 * sizeof(float))},
            {4, 4, VertexAttributeType::Float, 0, sizeof(Vertex2D), (const void *)(7 * sizeof(float))}},
        IndexBuffer(MaxSquaresNumber * sizeof(unsigned int) * 6));
    AddVertex("Squares", squaresVAO);

    static int MaxCubesNumber = 10000;
    auto &&cubesVAO = std::make_shared<VertexArray>(
        VertexBuffer(MaxCubesNumber * sizeof(Vertex3D) * 36),
        std::vector<VertexAttribute>{
            {0, 1, VertexAttributeType::UnsignedInt, 0, sizeof(Vertex3D), (const void *)(0 * sizeof(float))},
            {1, 3, VertexAttributeType::Float, 0, sizeof(Vertex3D), (const void *)(1 * sizeof(float))},
            {2, 3, VertexAttributeType::Float, 0, sizeof(Vertex3D), (const void *)(4 * sizeof(float))},
            {3, 3, VertexAttributeType::Float, 0, sizeof(Vertex3D), (const void *)(7 * sizeof(float))},
            {4, 4, VertexAttributeType::Float, 0, sizeof(Vertex3D), (const void *)(10 * sizeof(float))},
            {5, 4, VertexAttributeType::Float, 0, sizeof(Vertex3D), (const void *)(14 * sizeof(float))}});
    AddVertex("Cubes", cubesVAO);

    auto &&vertexArray = std::make_shared<VertexArray>(
        Engine::VertexBuffer(g_SkyboxCubeData.vertices, sizeof(g_SkyboxCubeData.vertices)),
        std::vector<VertexAttribute>{{0, 3, VertexAttributeType::Float, 0, 0, (const void *)0}});
    AddVertex("Skybox", vertexArray);

    auto &&CubeVAO = std::make_shared<VertexArray>(
        Engine::VertexBuffer(g_CubeData.Positions, sizeof(g_CubeData.Positions)),
        std::vector<VertexAttribute>{{0, 3, VertexAttributeType::Float, 0, 0, (const void *)0}});
    AddVertex("Cube", CubeVAO);

    LOG_ENGINE_INFO("VAO Library initialized");
}
