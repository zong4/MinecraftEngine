#pragma once

#include <Function.hpp>

namespace Editor
{

class EditorScene : public Engine::Scene
{
public:
    EditorScene();
    ~EditorScene() override = default;
};

} // namespace Editor