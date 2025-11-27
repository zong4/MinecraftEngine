#pragma once

#include "../ECS/Entity/Entity.hpp"
#include "../Scene/Scene.hpp"
#include "Algorithms/Random.hpp"

namespace Engine
{

class RayTracing
{
public:
    static void RenderScene(const Entity &camera, int raysPerPixel, int rayBounces,
                            std::vector<glm::vec4> &frameBuffer);
    static void SaveImage(const std::string &filepath, int width, int height,
                          const std::vector<glm::vec4> &frameBuffer);

private:
    static glm::vec3 RenderPixel(const Entity &camera, int x, int y, int raysPerPixel, int rayBounces);
};

} // namespace Engine