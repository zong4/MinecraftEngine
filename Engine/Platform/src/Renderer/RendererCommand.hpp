#pragma once

#include <Core.hpp>

namespace Engine
{

class RendererCommand
{
public:
    static void Init();

    static void GetError(const std::string &functionName);

public:
    static void SetClearColor(const glm::vec4 &color);
    static void Clear();
    static void ClearColorBuffer();
    static void ClearDepthBuffer();

    static void EnableDepthTest();
    static void DisableDepthTest();

    static void EnableBlend();
    static void DisableBlend();

    static void EnableFaceCulling();
    static void CullFrontFace();
    static void CullBackFace();
    static void DisableFaceCulling();

    static void EnableMultisampling();
    static void DisableMultisampling();

    static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height);
};

} // namespace Engine