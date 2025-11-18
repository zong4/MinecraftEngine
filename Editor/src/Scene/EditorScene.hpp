#pragma once

#include <Function.hpp>

namespace MCEditor
{

class EditorScene : public MCEngine::Scene
{
public:
    EditorScene();
    ~EditorScene() override = default;
};

} // namespace MCEditor