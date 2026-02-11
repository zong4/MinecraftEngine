#include "HierarchyPanel.hpp"

#include "../Scene/ScenesManager.hpp"
#include <imgui.h>

Editor::HierarchyPanel &Editor::HierarchyPanel::GetInstance()
{
    static HierarchyPanel instance;
    return instance;
}

void Editor::HierarchyPanel::OnImGuiRender()
{
    PROFILE_FUNCTION();

    ImGui::Begin("Hierarchy");

    // Draw all entities
    auto &&registry = ScenesManager::GetInstance().GetActiveScene()->GetRegistry();
    for (auto &&entity : registry.view<Engine::RelationshipComponent>())
    {
        if (!registry.get<Engine::RelationshipComponent>(entity).Parent)
            DrawEntityNode({entity, &registry});
    }

    // Context menu
    if (ImGui::BeginPopupContextWindow("HierarchyContextMenu",
                                       ImGuiPopupFlags_MouseButtonRight | ImGuiPopupFlags_NoOpenOverItems))
    {
        DrawContextMenu();
        ImGui::EndPopup();
    }

    ImGui::End();
}

void Editor::HierarchyPanel::DrawEntityNode(const Engine::Entity &entity)
{
    PROFILE_FUNCTION();

    if (!entity)
        return;

    ImGui::PushID((int)(uint32_t)entity);

    // Node flags
    ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_SpanAvailWidth;
    auto &&children = entity.GetComponent<Engine::RelationshipComponent>()->GetChildren();
    if (children.empty())
        node_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_Bullet;
    else
        node_flags |= ImGuiTreeNodeFlags_OpenOnArrow;

    // Selected
    bool opened = ImGui::TreeNodeEx(entity.GetComponent<Engine::LabelComponent>()->Name.c_str(), node_flags);
    if (ImGui::IsItemClicked())
        ScenesManager::GetInstance().SetSelectedEntity(entity);

    // Context menu
    if (ImGui::BeginPopupContextItem())
    {
        if (ImGui::MenuItem("Delete Entity"))
        {
            ScenesManager::GetInstance().GetActiveScene()->DeleteEntity(entity);
            ScenesManager::GetInstance().SetSelectedEntity(Engine::Entity());
        }
        ImGui::Separator();
        DrawContextMenu(entity);
        ImGui::EndPopup();
    }

    // Draw children
    if (opened)
    {
        for (auto &&child : children)
            DrawEntityNode(child);
        ImGui::TreePop();
    }

    ImGui::PopID();
}

void Editor::HierarchyPanel::DrawContextMenu(const Engine::Entity &parent)
{
    PROFILE_FUNCTION();

    if (ImGui::MenuItem("Create Empty Entity"))
    {
        DrawContextItem(parent,
                        []() { return ScenesManager::GetInstance().GetActiveScene()->AddEmptyEntity("EmptyEntity"); });
    }

    ImGui::Separator();

    if (ImGui::MenuItem("Create Square"))
    {
        DrawContextItem(parent, []() {
            return ScenesManager::GetInstance().GetActiveScene()->AddSquare("Square", Engine::TransformComponent());
        });
    }

    ImGui::Separator();

    if (ImGui::MenuItem("Create Cube"))
    {
        DrawContextItem(parent, []() {
            return ScenesManager::GetInstance().GetActiveScene()->AddCube("Cube", Engine::TransformComponent());
        });
    }

    ImGui::Separator();

    if (ImGui::MenuItem("Create Orthographic Camera"))
    {
        DrawContextItem(parent, []() {
            return ScenesManager::GetInstance().GetActiveScene()->AddCamera(
                "OrthographicCamera", Engine::TransformComponent(),
                Engine::CameraComponent(Engine::CameraType::Orthographic));
        });
    }
    if (ImGui::MenuItem("Create Perspective Camera"))
    {
        DrawContextItem(parent, []() {
            return ScenesManager::GetInstance().GetActiveScene()->AddCamera(
                "PerspectiveCamera", Engine::TransformComponent(),
                Engine::CameraComponent(Engine::CameraType::Perspective));
        });
    }

    ImGui::Separator();

    if (ImGui::MenuItem("Create Directional Light"))
    {
        DrawContextItem(parent, []() {
            return ScenesManager::GetInstance().GetActiveScene()->AddLight(
                "DirectionalLight", Engine::TransformComponent(),
                Engine::LightComponent(Engine::LightType::Directional));
        });
    }
    if (ImGui::MenuItem("Create Point Light"))
    {
        DrawContextItem(parent, []() {
            return ScenesManager::GetInstance().GetActiveScene()->AddLight(
                "PointLight", Engine::TransformComponent(), Engine::LightComponent(Engine::LightType::Point));
        });
    }
    if (ImGui::MenuItem("Create Spot Light"))
    {
        DrawContextItem(parent, []() {
            return ScenesManager::GetInstance().GetActiveScene()->AddLight(
                "SpotLight", Engine::TransformComponent(), Engine::LightComponent(Engine::LightType::Spot));
        });
    }
}

void Editor::HierarchyPanel::DrawContextItem(const Engine::Entity &parent,
                                             std::function<Engine::Entity()> createFunction)
{
    auto newEntity = createFunction();
    ScenesManager::GetInstance().SetSelectedEntity(newEntity);
    Engine::RelationshipComponent::SetParentChild(parent, newEntity);
}