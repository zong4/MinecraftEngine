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
    void Resize(int width, int height) override;

protected:
    void RenderColorID() const override;

private:
    int m_CubesCount = 0;

    // Ray tracing
    std::atomic<bool> m_RayTracingRunning = false;
    std::mutex m_RTMutex;
    std::condition_variable m_RTCV;
    std::thread m_RayTracingThread;
    std::vector<glm::vec4> m_RayTracingFrameBuffer;

private:
    void UploadCubesData();
    void RenderShadowMap() const;
    void Render3D(const Entity &camera) const;
    void RenderSkybox() const;
};

} // namespace Engine
