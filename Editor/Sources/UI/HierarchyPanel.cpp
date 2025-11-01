#include "HierarchyPanel.hpp"

#include "Manager/SceneManager.hpp"
#include <imgui.h>

void MCEditor::HierarchyPanel::OnImGuiRender()
{
    ENGINE_PROFILE_FUNCTION();

    ImGui::Begin("Hierarchy");

    auto &&registry = SceneManager::GetInstance().GetActiveScene()->GetRegistry();
    for (auto &&entity : registry.view<MCEngine::RelationshipComponent>())
    {
        if (!registry.get<MCEngine::RelationshipComponent>(entity).Parent)
            DrawEntityNode({entity, &registry});
    }

    if (ImGui::BeginPopupContextWindow("HierarchyContextMenu",
                                       ImGuiPopupFlags_MouseButtonRight | ImGuiPopupFlags_NoOpenOverItems))
    {
        if (ImGui::MenuItem("Create Empty Entity"))
        {
            auto newEntity = SceneManager::GetInstance().GetActiveScene()->AddEmptyEntity("EmptyEntity");
            SceneManager::GetInstance().SetSelectedEntity(newEntity);
        }

        ImGui::Separator();

        if (ImGui::MenuItem("Create Square"))
        {
            auto newEntity = SceneManager::GetInstance().GetActiveScene()->Add2DObject(
                "Square", MCEngine::TransformComponent(), MCEngine::SpriteRendererComponent());
            SceneManager::GetInstance().SetSelectedEntity(newEntity);
        }

        ImGui::Separator();

        if (ImGui::MenuItem("Create Cube"))
        {
            auto newEntity = SceneManager::GetInstance().GetActiveScene()->Add3DObject(
                "Cube", MCEngine::TransformComponent(), MCEngine::MeshRendererComponent());
            SceneManager::GetInstance().SetSelectedEntity(newEntity);
        }

        ImGui::Separator();

        if (ImGui::MenuItem("Create Orthographic Camera"))
        {
            auto newEntity = SceneManager::GetInstance().GetActiveScene()->AddCamera(
                "OrthographicCamera", MCEngine::TransformComponent(),
                MCEngine::CameraComponent(MCEngine::CameraType::Orthographic));
            SceneManager::GetInstance().SetSelectedEntity(newEntity);
        }

        if (ImGui::MenuItem("Create Perspective Camera"))
        {
            auto newEntity = SceneManager::GetInstance().GetActiveScene()->AddCamera(
                "PerspectiveCamera", MCEngine::TransformComponent(),
                MCEngine::CameraComponent(MCEngine::CameraType::Perspective));
            SceneManager::GetInstance().SetSelectedEntity(newEntity);
        }

        ImGui::Separator();

        if (ImGui::MenuItem("Create Directional Light"))
        {
            auto newEntity = SceneManager::GetInstance().GetActiveScene()->AddLight(
                "DirectionalLight", MCEngine::TransformComponent(),
                MCEngine::LightComponent(MCEngine::LightType::Directional));
            SceneManager::GetInstance().SetSelectedEntity(newEntity);
        }

        if (ImGui::MenuItem("Create Point Light"))
        {
            auto newEntity = SceneManager::GetInstance().GetActiveScene()->AddLight(
                "PointLight", MCEngine::TransformComponent(), MCEngine::LightComponent(MCEngine::LightType::Point));
            SceneManager::GetInstance().SetSelectedEntity(newEntity);
        }

        if (ImGui::MenuItem("Create Spot Light"))
        {
            auto newEntity = SceneManager::GetInstance().GetActiveScene()->AddLight(
                "SpotLight", MCEngine::TransformComponent(), MCEngine::LightComponent(MCEngine::LightType::Spot));
            SceneManager::GetInstance().SetSelectedEntity(newEntity);
        }

        ImGui::EndPopup();
    }

    ImGui::End();
}

void MCEditor::HierarchyPanel::DrawEntityNode(MCEngine::Entity entity)
{
    ENGINE_PROFILE_FUNCTION();

    if (!entity)
        return;

    ImGui::PushID((int)(uint32_t)entity);

    ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_SpanAvailWidth;
    auto &&children = entity.GetComponent<MCEngine::RelationshipComponent>().GetChildren();
    if (children.empty())
        node_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_Bullet;
    else
        node_flags |= ImGuiTreeNodeFlags_OpenOnArrow;

    bool opened = ImGui::TreeNodeEx(entity.GetComponent<MCEngine::TagComponent>().Tag.c_str(), node_flags);
    if (ImGui::IsItemClicked())
        SceneManager::GetInstance().SetSelectedEntity(entity);

    if (ImGui::BeginPopupContextItem())
    {
        // if (ImGui::MenuItem("Rename Entity"))
        // {
        //     ImGui::OpenPopup("RenameEntity");
        // }

        if (ImGui::MenuItem("Delete Entity"))
        {
            SceneManager::GetInstance().GetActiveScene()->DeleteEntity(entity);
            SceneManager::GetInstance().SetSelectedEntity(MCEngine::Entity());
        }

        ImGui::EndPopup();
    }

    if (opened)
    {
        for (auto &&child : children)
            DrawEntityNode(child);
        ImGui::TreePop();
    }

    ImGui::PopID();
}