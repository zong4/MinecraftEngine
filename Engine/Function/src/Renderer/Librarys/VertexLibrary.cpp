#include "VertexLibrary.hpp"

#include <nlohmann/json.hpp>

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

std::string Engine::VertexLibrary::GetName(const std::shared_ptr<VertexArray> &vertexArray) const
{
    for (const auto &[name, ptr] : m_VertexMap)
    {
        if (ptr == vertexArray)
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

void Engine::VertexLibrary::AddVertex(const std::string &name, const std::shared_ptr<VertexArray> &vertexArray)
{
    if (!vertexArray)
    {
        LOG_ENGINE_ASSERT("Cannot add null vertex array: " + name);
        return;
    }

    if (Exists(name))
    {
        LOG_ENGINE_WARN("VAO already exists: " + name);
        return;
    }
    m_VertexMap[name] = vertexArray;
    LOG_ENGINE_TRACE("VAO added: " + name);
}

Engine::VertexLibrary::VertexLibrary()
{
    PROFILE_FUNCTION();

    auto &&CubeVAO =
        VertexArray::Create(VertexBuffer::Create(g_CubeData.Positions, sizeof(g_CubeData.Positions)),
                            std::vector<VertexAttribute>{{0, 3, VertexAttributeType::Float, 0, 0, (const void *)0}});
    AddVertex("Cube", CubeVAO);

    auto &&vertexArray =
        VertexArray::Create(VertexBuffer::Create(g_SkyboxCubeData.vertices, sizeof(g_SkyboxCubeData.vertices)),
                            std::vector<VertexAttribute>{{0, 3, VertexAttributeType::Float, 0, 0, (const void *)0}});
    AddVertex("Skybox", vertexArray);

    // Create dynamic VertexArrays
    ReadConfig();
    {
        auto &&squaresVAO = VertexArray::Create(
            VertexBuffer::Create(m_MaxSquaresNumber * sizeof(Vertex2D) * 4),
            std::vector<VertexAttribute>{
                {0, 1, VertexAttributeType::UInt, 0, sizeof(Vertex2D), (const void *)(0 * sizeof(float))},
                {1, 3, VertexAttributeType::Float, 0, sizeof(Vertex2D), (const void *)(1 * sizeof(float))},
                {2, 2, VertexAttributeType::Float, 0, sizeof(Vertex2D), (const void *)(4 * sizeof(float))},
                {3, 1, VertexAttributeType::Int, 0, sizeof(Vertex2D), (const void *)(6 * sizeof(float))},
                {4, 4, VertexAttributeType::Float, 0, sizeof(Vertex2D), (const void *)(7 * sizeof(float))}},
            Engine::IndexBuffer::Create(m_MaxSquaresNumber * 6 * sizeof(unsigned int)));
        AddVertex("Squares", squaresVAO);

        auto &&cubesVAO = VertexArray::Create(
            VertexBuffer::Create(m_MaxCubesNumber * sizeof(Vertex3D) * 36),
            std::vector<VertexAttribute>{
                {0, 1, VertexAttributeType::UInt, 0, sizeof(Vertex3D), (const void *)(0 * sizeof(float))},
                {1, 3, VertexAttributeType::Float, 0, sizeof(Vertex3D), (const void *)(1 * sizeof(float))},
                {2, 3, VertexAttributeType::Float, 0, sizeof(Vertex3D), (const void *)(4 * sizeof(float))},
                {3, 3, VertexAttributeType::Float, 0, sizeof(Vertex3D), (const void *)(7 * sizeof(float))},
                {4, 4, VertexAttributeType::Float, 0, sizeof(Vertex3D), (const void *)(10 * sizeof(float))},
                {5, 4, VertexAttributeType::Float, 0, sizeof(Vertex3D), (const void *)(14 * sizeof(float))}});
        AddVertex("Cubes", cubesVAO);
    }

    LOG_ENGINE_INFO("VAO Library initialized");
}

void Engine::VertexLibrary::ReadConfig()
{
    PROFILE_FUNCTION();

    nlohmann::json config;
    std::ifstream configFile(std::string(PROJECT_ROOT) + "/Engine/Function/config/ResourcesManager.json");
    if (configFile.is_open())
    {
        configFile >> config;
        configFile.close();

        m_MaxSquaresNumber = config["MaxSquaresNumber"].get<int>();
        m_MaxCubesNumber = config["MaxCubesNumber"].get<int>();
    }
    else
    {
        LOG_ENGINE_WARN("Could not open ResourcesManager.json, using default values");
        m_MaxSquaresNumber = 10000;
        m_MaxCubesNumber = 10000;
    }
}