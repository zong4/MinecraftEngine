#pragma once

#include "ImGuiLayer.hpp"

namespace Engine
{

class OpenGLImGuiLayer : public ImGuiLayer
{
public:
    OpenGLImGuiLayer(void *nativeWindow) : ImGuiLayer(nativeWindow) {}
    virtual ~OpenGLImGuiLayer() override = default;

public:
    void BeginRenderImGui() const override;
    void EndRenderImGui() const override;

protected:
    void InitRenderer() override;
    void ShutdownRenderer() override;
};

} // namespace Engine