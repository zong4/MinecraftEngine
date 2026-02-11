#pragma once

#include <Function.hpp>

namespace Editor
{

class HierarchyPanel
{
public:
    static HierarchyPanel &GetInstance();

public:
    void OnImGuiRender();

private:
    void DrawEntityNode(const Engine::Entity &entity);
    void DrawContextMenu(const Engine::Entity &parent = Engine::Entity());
    static void DrawContextItem(const Engine::Entity &parent, std::function<Engine::Entity()> createFunction);

private:
    HierarchyPanel() = default;
    ~HierarchyPanel() = default;
};

} // namespace Editor