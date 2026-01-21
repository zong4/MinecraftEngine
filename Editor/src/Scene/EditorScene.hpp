#pragma once

#include <Function.hpp>

namespace Editor
{

class EditorScene : public Engine::Scene3D
{
public:
    EditorScene();
    ~EditorScene() override = default;
};

} // namespace Editor