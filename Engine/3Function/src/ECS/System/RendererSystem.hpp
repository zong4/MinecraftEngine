#pragma once

#include "../Component/CameraComponent.hpp"
#include "../Component/LightComponent.hpp"
#include "../Component/MaterialComponent.hpp"
#include "../Component/RendererComponents.hpp"
#include "../Component/TransformComponent.hpp"
#include <entt/entt.hpp>

namespace Engine
{

class RendererSystem
{
public:
    RendererSystem();
    ~RendererSystem() = default;

public:
    void Resize(entt::registry &registry, int width, int height);
    void Render(entt::registry &registry);
    void RenderSkybox() const;

private:
    //  2D
    int m_SquaresCount = 0;

    // 3D
    int m_GrassCubesCount = 0;
    int m_StoneCubesCount = 0;
    std::shared_ptr<TextureCube> m_SkyboxTexture;

    // Common
    std::shared_ptr<FrameBuffer> m_ColorIDFrameBuffer = FrameBuffer::Create(Texture2DType::Integer, 1280, 720);

private:
    // 2D
    void UploadSquares(entt::registry &registry);
    void Render2D(entt::registry &registry) const;

    // 3D
    void UploadCubes(entt::registry &registry);
    void RenderShadowMap(entt::registry &registry) const;
    void Render3D(entt::registry &registry) const;

    // Common
    void RenderColorID() const;
};

} // namespace Engine