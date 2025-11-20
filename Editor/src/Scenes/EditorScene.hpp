#pragma once

#include <Function.hpp>

namespace Editor
{

class EditorScene : public MCEngine::Scene
{
public:
    EditorScene();
    ~EditorScene() override = default;
};

} // namespace Editor