#pragma once

#include "pch.hpp"

namespace MCEngine
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
};

} // namespace MCEngine