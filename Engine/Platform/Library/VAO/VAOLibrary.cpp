#include "VAOLibrary.hpp"

#include <glad/glad.h>

namespace MCEngine
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

} // namespace MCEngine

MCEngine::VAOLibrary &MCEngine::VAOLibrary::GetInstance()
{
    static VAOLibrary instance;
    return instance;
}

std::string MCEngine::VAOLibrary::GetName(const std::shared_ptr<VertexArray> &vao) const
{
    for (const auto &[name, ptr] : m_VAOs)
    {
        if (ptr == vao)
            return name;
    }
    LOG_ENGINE_WARN("VAO not found in library");
    return "";
}

std::shared_ptr<MCEngine::VertexArray> MCEngine::VAOLibrary::GetVAO(const std::string &name)
{
    ENGINE_PROFILE_FUNCTION();

    if (!Exists(name))
    {
        LOG_ENGINE_ERROR("VAO not found: " + name);
        return nullptr;
    }
    return m_VAOs[name];
}

void MCEngine::VAOLibrary::AddVAO(const std::string &name, const std::shared_ptr<VertexArray> &vao)
{
    ENGINE_PROFILE_FUNCTION();

    if (Exists(name))
    {
        LOG_ENGINE_WARN("VAO already exists: " + name);
        return;
    }
    m_VAOs[name] = vao;
    LOG_ENGINE_TRACE("VAO added: " + name);
}

MCEngine::VAOLibrary::VAOLibrary()
{
    ENGINE_PROFILE_FUNCTION();

    static int MaxSquaresNumber = 10000;
    auto &&squaresVAO = std::make_shared<VertexArray>(
        VertexBuffer(MaxSquaresNumber * sizeof(Vertex2D) * 4),
        std::vector<VertexAttribute>{
            {0, 1, GL_UNSIGNED_INT, GL_FALSE, sizeof(Vertex2D), (const void *)(0 * sizeof(float))},
            {1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), (const void *)(1 * sizeof(float))},
            {2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), (const void *)(4 * sizeof(float))},
            {3, 1, GL_INT, GL_FALSE, sizeof(Vertex2D), (const void *)(6 * sizeof(float))},
            {4, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), (const void *)(7 * sizeof(float))}},
        IndexBuffer(MaxSquaresNumber * sizeof(unsigned int) * 6));
    AddVAO("Squares", squaresVAO);

    static int MaxCubesNumber = 10000;
    auto &&cubesVAO = std::make_shared<VertexArray>(
        VertexBuffer(MaxCubesNumber * sizeof(Vertex3D) * 36),
        std::vector<VertexAttribute>{
            {0, 1, GL_UNSIGNED_INT, GL_FALSE, sizeof(Vertex3D), (const void *)(0 * sizeof(float))},
            {1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), (const void *)(1 * sizeof(float))},
            {2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), (const void *)(4 * sizeof(float))},
            {3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), (const void *)(7 * sizeof(float))},
            {4, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), (const void *)(10 * sizeof(float))},
            {5, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), (const void *)(14 * sizeof(float))}});
    AddVAO("Cubes", cubesVAO);

    auto &&vertexArray = std::make_shared<VertexArray>(
        MCEngine::VertexBuffer(g_SkyboxCubeData.vertices, sizeof(g_SkyboxCubeData.vertices)),
        std::vector<VertexAttribute>{{0, 3, GL_FLOAT, GL_FALSE, 0, (const void *)0}});
    AddVAO("Skybox", vertexArray);

    LOG_ENGINE_INFO("VAO Library initialized");
}

bool MCEngine::VAOLibrary::Exists(const std::string &name) const { return m_VAOs.find(name) != m_VAOs.end(); }