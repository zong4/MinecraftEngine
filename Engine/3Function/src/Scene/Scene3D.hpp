#pragma once

#include "Scene.hpp"

namespace Engine
{

class Scene3D : public Scene
{
public:
    Scene3D(const std::string &name = "Untitled");
    virtual ~Scene3D() override = default;

public:
    void Render(const Entity &camera) override;

private:
    // Ray tracing
    std::atomic<bool> m_RayTracingRunning = false;
    std::mutex m_RTMutex;
    std::condition_variable m_RTCV;
    std::thread m_RayTracingThread;
    std::vector<glm::vec4> m_RayTracingFrameBuffer;

private:
    void RenderSkybox() const;
};

} // namespace Engine
