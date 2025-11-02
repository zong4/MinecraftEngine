#include "HierarchyPanel.hpp"

#include "Manager/SceneManager.hpp"
#include <imgui.h>

void MCEditor::HierarchyPanel::OnImGuiRender()
{
    ENGINE_PROFILE_FUNCTION();

    ImGui::Begin("Hierarchy");

    // Draw all entities
    auto &&registry = SceneManager::GetInstance().GetActiveScene()->GetRegistry();
    for (auto &&entity : registry.view<MCEngine::RelationshipComponent>())
    {
        if (!registry.get<MCEngine::RelationshipComponent>(entity).GetParent())
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

void MCEditor::HierarchyPanel::DrawEntityNode(const MCEngine::Entity &entity)
{
    ENGINE_PROFILE_FUNCTION();

    if (!entity)
        return;

    ImGui::PushID((int)(uint32_t)entity);

    // Node flags
    ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_SpanAvailWidth;
    auto &&children = entity.GetComponent<MCEngine::RelationshipComponent>()->GetChildren();
    if (children.empty())
        node_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_Bullet;
    else
        node_flags |= ImGuiTreeNodeFlags_OpenOnArrow;

    // Selected
    bool opened = ImGui::TreeNodeEx(entity.GetComponent<MCEngine::TagComponent>()->Tag.c_str(), node_flags);
    if (ImGui::IsItemClicked())
        SceneManager::GetInstance().SetSelectedEntity(entity);

    // Context menu
    if (ImGui::BeginPopupContextItem())
    {
        if (ImGui::MenuItem("Delete Entity"))
        {
            SceneManager::GetInstance().GetActiveScene()->DeleteEntity(entity);
            SceneManager::GetInstance().SetSelectedEntity(MCEngine::Entity());
        }
        ImGui::Separator();
        DrawContextMenu();

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

void MCEditor::HierarchyPanel::DrawContextMenu(const MCEngine::Entity &parent)
{
    ENGINE_PROFILE_FUNCTION();

    if (ImGui::MenuItem("Create Empty Entity"))
    {
        DrawContextItem(parent,
                        []() { return SceneManager::GetInstance().GetActiveScene()->AddEmptyEntity("EmptyEntity"); });
    }

    ImGui::Separator();

    if (ImGui::MenuItem("Create Square"))
    {
        DrawContextItem(parent, []() {
            return SceneManager::GetInstance().GetActiveScene()->Add2DObject("Square", MCEngine::TransformComponent(),
                                                                             MCEngine::SpriteRendererComponent());
        });
    }

    ImGui::Separator();

    if (ImGui::MenuItem("Create Cube"))
    {
        DrawContextItem(parent, []() {
            return SceneManager::GetInstance().GetActiveScene()->Add3DObject("Cube", MCEngine::TransformComponent(),
                                                                             MCEngine::MeshRendererComponent());
        });
    }

    ImGui::Separator();

    if (ImGui::MenuItem("Create Orthographic Camera"))
    {
        DrawContextItem(parent, []() {
            return SceneManager::GetInstance().GetActiveScene()->AddCamera(
                "OrthographicCamera", MCEngine::TransformComponent(),
                MCEngine::CameraComponent(MCEngine::CameraType::Orthographic));
        });
    }

    if (ImGui::MenuItem("Create Perspective Camera"))
    {
        DrawContextItem(parent, []() {
            return SceneManager::GetInstance().GetActiveScene()->AddCamera(
                "PerspectiveCamera", MCEngine::TransformComponent(),
                MCEngine::CameraComponent(MCEngine::CameraType::Perspective));
        });
    }

    ImGui::Separator();

    if (ImGui::MenuItem("Create Directional Light"))
    {
        DrawContextItem(parent, []() {
            return SceneManager::GetInstance().GetActiveScene()->AddLight(
                "DirectionalLight", MCEngine::TransformComponent(),
                MCEngine::LightComponent(MCEngine::LightType::Directional));
        });
    }
    if (ImGui::MenuItem("Create Point Light"))
    {
        DrawContextItem(parent, []() {
            return SceneManager::GetInstance().GetActiveScene()->AddLight(
                "PointLight", MCEngine::TransformComponent(), MCEngine::LightComponent(MCEngine::LightType::Point));
        });
    }
    if (ImGui::MenuItem("Create Spot Light"))
    {
        DrawContextItem(parent, []() {
            return SceneManager::GetInstance().GetActiveScene()->AddLight(
                "SpotLight", MCEngine::TransformComponent(), MCEngine::LightComponent(MCEngine::LightType::Spot));
        });
    }
}

void MCEditor::HierarchyPanel::DrawContextItem(const MCEngine::Entity &parent,
                                               std::function<MCEngine::Entity()> createFunction)
{
    auto newEntity = createFunction();
    SceneManager::GetInstance().SetSelectedEntity(newEntity);
    MCEngine::RelationshipComponent::SetParentChild(parent, newEntity);
}