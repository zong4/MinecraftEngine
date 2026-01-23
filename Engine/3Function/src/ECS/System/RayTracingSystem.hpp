#pragma once

#include "../Component/CameraComponent.hpp"
#include "../Component/RendererComponents.hpp"
#include "../Component/TransformComponent.hpp"
#include "../Entity/Entity.hpp"
#include <entt/entt.hpp>

namespace Engine
{

struct RayTracingObject
{
    glm::mat4 transform;
    BoundingBox bbox;
};

class RayTracingSystem
{
public:
    static RayTracingSystem &GetInstance();

public:
    void Render(entt::registry &registry, const Entity &camera, int raysPerPixel, int rayBounces);
    void SaveImage(const std::string &filepath);

private:
    // Multithreading
    std::atomic<bool> m_Running = false;
    std::mutex m_Mutex;
    std::condition_variable m_CV;
    std::thread m_Thread;

    // Common
    int m_Width, m_Height;
    std::vector<RayTracingObject> m_Objects;
    std::vector<glm::vec4> m_FrameBuffer;

private:
    RayTracingSystem() = default;
    ~RayTracingSystem();

private:
    glm::vec3 RenderPixel(const Entity &camera, int raysPerPixel, int rayBounces, int x, int y);
};

} // namespace Engine