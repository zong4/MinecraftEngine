#pragma once

#include "../Component/LightComponent.hpp"
#include "../Component/MaterialComponent.hpp"
#include "../Component/RendererComponents.hpp"
#include "../Component/TransformComponent.hpp"
#include <Platform.hpp>
#include <entt/entt.hpp>

namespace Engine
{

class RendererSystem
{
public:
    static RendererSystem &GetInstance();

    // Getters
    int GetSquaresCount() const { return m_SquaresCount; }
    int GetCubesCount() const { return m_CubesCount; }

public:
    void Resize(int width, int height) { m_ColorIDFrameBuffer->Resize(width, height); }
    void Update(entt::registry &registry);
    void Upload(entt::registry &registry);
    void Render(entt::registry &registry) const;
    void RenderSkybox() const;

private:
    int m_SquaresCount = 0;
    int m_CubesCount = 0;
    std::shared_ptr<FrameBuffer> m_ColorIDFrameBuffer = FrameBuffer::Create(Texture2DType::Integer, 1280, 720);

    // Only for 3D scenes
    std::shared_ptr<TextureCube> m_SkyboxTexture;

private:
    RendererSystem();
    ~RendererSystem() = default;

private:
    void UploadSquares(entt::registry &registry);
    void UploadCubes(entt::registry &registry);
    void Render2D(entt::registry &registry) const;
    void Render3D(entt::registry &registry) const;
    void RenderColorID() const;
};

} // namespace Engine