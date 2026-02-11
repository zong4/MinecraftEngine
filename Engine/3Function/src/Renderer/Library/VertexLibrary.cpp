#include "VertexLibrary.hpp"

#include <nlohmann/json.hpp>

Engine::VertexLibrary &Engine::VertexLibrary::GetInstance()
{
    static VertexLibrary instance;
    return instance;
}

std::string Engine::VertexLibrary::GetName(const std::shared_ptr<VertexArray> &vertexArray) const
{
    for (const auto &[name, ptr] : m_VertexsMap)
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
    return m_VertexsMap[name];
}

void Engine::VertexLibrary::AddVertex(const std::string &name, const std::shared_ptr<VertexArray> &vertexArray)
{
    // Nullptr check
    if (!vertexArray)
    {
        LOG_ENGINE_ERROR("Cannot add null vertex array: " + name);
        return;
    }

    // Check for duplicates
    if (Exists(name))
    {
        LOG_ENGINE_WARN("VAO already exists: " + name + ", overwriting");
    }

    // Add to map
    m_VertexsMap[name] = vertexArray;
    LOG_ENGINE_TRACE("VAO added: " + name);
}

Engine::VertexLibrary::VertexLibrary()
{
    PROFILE_FUNCTION();

    // Create static VertexArrays
    auto &&CubeVAO = VertexArray::Create(
        VertexBuffer::Create(g_CubeData.Positions, sizeof(g_CubeData.Positions)),
        std::vector<VertexAttribute>{{0, 3, VertexAttributeType::Float, false, 3 * sizeof(float), (const void *)0}});
    AddVertex("Cube", CubeVAO);

    // Create dynamic VertexArrays
    ReadConfig();
    {
        auto &&squaresVAO =
            VertexArray::Create(VertexBuffer::Create(m_MaxSquaresNumber * sizeof(Vertex2D) * 4),
                                std::vector<VertexAttribute>{{0, 1, VertexAttributeType::UInt, false, sizeof(Vertex2D),
                                                              (const void *)(0 * sizeof(float))}, // EntityID
                                                             {1, 3, VertexAttributeType::Float, false, sizeof(Vertex2D),
                                                              (const void *)(1 * sizeof(float))}, // Position
                                                             {2, 4, VertexAttributeType::Float, false, sizeof(Vertex2D),
                                                              (const void *)(4 * sizeof(float))}, // Color
                                                             {3, 2, VertexAttributeType::Float, false, sizeof(Vertex2D),
                                                              (const void *)(8 * sizeof(float))}, // TexCoord
                                                             {4, 1, VertexAttributeType::Int, false, sizeof(Vertex2D),
                                                              (const void *)(10 * sizeof(float))}}, // TexIndex
                                Engine::IndexBuffer::Create(m_MaxSquaresNumber * 6 * sizeof(unsigned int)));
        auto &&grassCubesVAO =
            VertexArray::Create(VertexBuffer::Create(m_MaxCubesNumber * sizeof(Vertex3D) * 36),
                                std::vector<VertexAttribute>{{0, 1, VertexAttributeType::UInt, false, sizeof(Vertex3D),
                                                              (const void *)(0 * sizeof(float))}, // EntityID
                                                             {1, 3, VertexAttributeType::Float, false, sizeof(Vertex3D),
                                                              (const void *)(1 * sizeof(float))}, // Position
                                                             {2, 3, VertexAttributeType::Float, false, sizeof(Vertex3D),
                                                              (const void *)(4 * sizeof(float))}, // Normal
                                                             {3, 4, VertexAttributeType::Float, false, sizeof(Vertex3D),
                                                              (const void *)(7 * sizeof(float))}, // Material
                                                             {4, 4, VertexAttributeType::Float, false, sizeof(Vertex3D),
                                                              (const void *)(11 * sizeof(float))}, // Color
                                                             {5, 3, VertexAttributeType::Float, false, sizeof(Vertex3D),
                                                              (const void *)(15 * sizeof(float))}, // TexCoord
                                                             {6, 1, VertexAttributeType::Int, false, sizeof(Vertex3D),
                                                              (const void *)(18 * sizeof(float))}}); // TexIndex
        auto &&StoneCubesVAO =
            VertexArray::Create(VertexBuffer::Create(m_MaxCubesNumber * sizeof(Vertex3D) * 36),
                                std::vector<VertexAttribute>{{0, 1, VertexAttributeType::UInt, false, sizeof(Vertex3D),
                                                              (const void *)(0 * sizeof(float))}, // EntityID
                                                             {1, 3, VertexAttributeType::Float, false, sizeof(Vertex3D),
                                                              (const void *)(1 * sizeof(float))}, // Position
                                                             {2, 3, VertexAttributeType::Float, false, sizeof(Vertex3D),
                                                              (const void *)(4 * sizeof(float))}, // Normal
                                                             {3, 4, VertexAttributeType::Float, false, sizeof(Vertex3D),
                                                              (const void *)(7 * sizeof(float))}, // Material
                                                             {4, 4, VertexAttributeType::Float, false, sizeof(Vertex3D),
                                                              (const void *)(11 * sizeof(float))}, // Color
                                                             {5, 3, VertexAttributeType::Float, false, sizeof(Vertex3D),
                                                              (const void *)(15 * sizeof(float))}, // TexCoord
                                                             {6, 1, VertexAttributeType::Int, false, sizeof(Vertex3D),
                                                              (const void *)(18 * sizeof(float))}}); // TexIndex
        AddVertex("Squares", squaresVAO);
        AddVertex("GrassCubes", grassCubesVAO);
        AddVertex("StoneCubes", StoneCubesVAO);
    }
    LOG_ENGINE_INFO("VAO Library initialized");
}

void Engine::VertexLibrary::ReadConfig()
{
    PROFILE_FUNCTION();

    nlohmann::json config;
    std::ifstream configFile(GetConfigsDirectory() / "ResourcesManager.json");
    if (configFile.is_open())
    {
        configFile >> config;
        configFile.close();
        m_MaxSquaresNumber = config["MaxSquaresNumber"].get<int>();
        m_MaxCubesNumber = config["MaxCubesNumber"].get<int>();
    }
    else
    {
        m_MaxSquaresNumber = 3000;
        m_MaxCubesNumber = 3000;
        LOG_ENGINE_WARN("Could not open ResourcesManager.json, using default values");
    }
}