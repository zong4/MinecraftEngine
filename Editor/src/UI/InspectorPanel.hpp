#pragma once

#include <Function.hpp>

namespace Editor
{

class InspectorPanel
{
public:
    static InspectorPanel &GetInstance();

public:
    void OnImGuiRender() const;

protected:
    static bool InputTextString(const char *label, std::string *str);
    static void DrawVec3Control(const std::string &label, glm::vec3 &values, float resetValue);
    static void DrawAddComponentButton(Engine::Entity selectedEntity);

private:
    InspectorPanel() = default;
    ~InspectorPanel() = default;
};

} // namespace Editor