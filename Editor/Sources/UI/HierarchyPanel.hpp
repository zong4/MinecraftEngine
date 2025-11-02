#pragma once

#include <Function.hpp>

namespace MCEditor
{

class HierarchyPanel
{
public:
    HierarchyPanel() = default;
    ~HierarchyPanel() = default;

public:
    void OnImGuiRender();

private:
    void DrawEntityNode(const MCEngine::Entity &entity);
    void DrawContextMenu(const MCEngine::Entity &parent = MCEngine::Entity());
    static void DrawContextItem(const MCEngine::Entity &parent, std::function<MCEngine::Entity()> createFunction);
};

} // namespace MCEditor