#pragma once

#include "../Component/CameraComponent.hpp"
#include "../Component/LabelComponent.hpp"
#include "../Component/RendererComponents.hpp"
#include "../Component/TransformComponent.hpp"
#include "../Entity/Entity.hpp"
#include <entt/entt.hpp>

namespace Engine
{

struct RayTracingObject
{
    std::string name;
    glm::mat4 transform;
    BoundingBox bbox;
};

class RayTracingSystem
{
public:
    static RayTracingSystem &GetInstance();

public:
    void Render(entt::registry &registry, const Entity &camera, int raysPerPixel, int rayBounces);

private:
    // Multithreading
    std::atomic<bool> m_Running = false;
    // std::mutex m_Mutex;
    // std::condition_variable m_CV;
    // std::thread m_Thread;

    // Common
    int m_Width, m_Height;
    TransformComponent m_CameraTransform;
    CameraComponent m_CameraComponent = CameraComponent(CameraType::Perspective);
    std::vector<RayTracingObject> m_Objects;
    std::vector<glm::vec4> m_FrameBuffer;

private:
    RayTracingSystem() = default;
    ~RayTracingSystem();

private:
    glm::vec3 RenderPixel(int raysPerPixel, int rayBounces, int x, int y);
    void SaveImage(const std::string &filepath);
};

} // namespace Engine