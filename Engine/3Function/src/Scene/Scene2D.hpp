#pragma once

#include "Scene.hpp"

namespace Engine
{

class Scene2D : public Scene
{
public:
    Scene2D(const std::string &name = "Untitled") : Scene(name) {}
    virtual ~Scene2D() override = default;

public:
    void Render(const Entity &camera) override;

protected:
    void RenderColorID() const override;

private:
    int m_SquaresCount = 0;

private:
    void UploadSquaresData();
    void Render2D(const Entity &camera) const;
};

} // namespace Engine
