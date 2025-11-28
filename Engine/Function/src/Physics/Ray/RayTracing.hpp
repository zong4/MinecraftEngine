#pragma once

#include "../../Algorithms/Random.hpp"
#include "../../ECS/Entity/Entity.hpp"
#include "../../Scene/Scene.hpp"

namespace Engine
{

struct RTMesh
{
    std::vector<glm::vec3> vertices;
    std::vector<uint32_t> indices;
    glm::mat4 transform;
};

struct RTSceneData
{
    std::vector<RTMesh> meshes;
};

class RayTracing
{
public:
    static void RenderScene(const Entity &camera, const RTSceneData &sceneData, int raysPerPixel, int rayBounces,
                            std::vector<glm::vec4> &frameBuffer);
    static void SaveImage(const std::string &filepath, int width, int height,
                          const std::vector<glm::vec4> &frameBuffer);

private:
    static glm::vec3 RenderPixel(const Entity &camera, const RTSceneData &sceneData, int x, int y, int raysPerPixel,
                                 int rayBounces);
};

} // namespace Engine