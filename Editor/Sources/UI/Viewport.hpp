#pragma once

#include "FileBrowserPanel.hpp"

namespace MCEditor
{

class Viewport
{
public:
    Viewport() = default;
    virtual ~Viewport() = default;

public:
    virtual void Render();
    virtual void OnImGuiRender();
    void ReceiveDrop(FileBrowserPanel &fileBrowserPanel);

protected:
    bool m_ViewportDirty = false;
    glm::vec2 m_ViewportSize;
    std::unique_ptr<MCEngine::FrameBuffer> m_FBO =
        std::make_unique<MCEngine::FrameBuffer>(MCEngine::FrameBufferType::Color, 1280, 720);
    std::unique_ptr<MCEngine::FrameBuffer> m_MultisampleFBO =
        std::make_unique<MCEngine::FrameBuffer>(MCEngine::FrameBufferType::Multisample, 1280, 720, 4);
};

} // namespace MCEditor