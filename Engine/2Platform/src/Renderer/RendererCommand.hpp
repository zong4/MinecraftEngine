#pragma once

#include <Core.hpp>

namespace Engine
{

enum class CullingFace
{
    None = 0,
    Front = 1,
    Back = 2
};

enum class DepthTestFunction
{
    Less = 0,
    LessEqual = 1,
    Greater = 2,
    GreaterEqual = 3,
    Equal = 4,
    NotEqual = 5,
    Always = 6,
    Never = 7
};

class RendererCommand
{
public:
    static void Init();
    static void GetError(const std::string &functionName);

public:
    // Window
    static void SetClearColor(const glm::vec4 &color);
    static void Clear();
    static void ClearColorBuffer();
    static void ClearDepthBuffer();
    static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height);

    // Preformance
    static void SetFaceCulling(CullingFace face);
    static void SetMultisampling(bool enabled);

    // Post-processing
    static void SetDepthTest(bool enabled);
    static void SetDepthWrite(bool enabled);
    static void SetDepthTestFunction(DepthTestFunction function);
    static void SetBlend(bool enabled);
    static void SetGammaCorrection(bool enabled);
};

} // namespace Engine