#pragma once

#include <Function.hpp>

namespace MCEditor
{
class InspectorPanel
{
public:
    InspectorPanel() = default;
    ~InspectorPanel() = default;

public:
    void OnImGuiRender() const;

protected:
    static bool InputTextString(const char *label, std::string *str);
    static void DrawVec3Control(const std::string &label, glm::vec3 &values, float resetValue);
    static void DrawAddComponentButton(MCEngine::Entity selectedEntity);
};
} // namespace MCEditor