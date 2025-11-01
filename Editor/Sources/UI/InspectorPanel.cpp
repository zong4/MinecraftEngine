#include "InspectorPanel.hpp"

#include "Manager/ConfigManager.hpp"
#include "Manager/SceneManager.hpp"
#include <imgui.h>
#include <imgui_internal.h>

// clang-format off
template <typename T, typename UIFunction>
// clang-format on
static void DrawTable2(const std::string &label, UIFunction &&uiFunction)
{
    ENGINE_PROFILE_FUNCTION();

    ImGui::BeginTable(label.c_str(), 2, ImGuiTableFlags_SizingStretchProp | ImGuiTableFlags_NoPadOuterX);
    ImGui::TableSetupColumn("Label", ImGuiTableColumnFlags_None, 0.16f);
    ImGui::TableSetupColumn("Value", ImGuiTableColumnFlags_None, 0.84f);
    ImGui::TableNextRow();

    // Label
    ImGui::TableSetColumnIndex(0);
    ImGui::TextUnformatted(label.c_str());

    // Value
    ImGui::TableSetColumnIndex(1);
    ImGui::SetNextItemWidth(-FLT_MIN);
    uiFunction();

    ImGui::EndTable();
}

// clang-format off
template <typename T, typename UIFunction>
// clang-format on
static void DrawComponent(const std::string &name, MCEngine::Entity entity, UIFunction &&uiFunction,
                          bool removeable = true)
{
    ENGINE_PROFILE_FUNCTION();

    if (entity && entity.HasComponent<T>())
    {
        std::string header = name + "##" + std::to_string(static_cast<uint32_t>(entity));

        if (removeable)
        {
            bool open = true;
            if (ImGui::CollapsingHeader(header.c_str(), &open, ImGuiTreeNodeFlags_DefaultOpen))
            {
                ImGui::PushID(header.c_str());
                uiFunction(entity.GetComponent<T>());
                ImGui::PopID();
            }

            if (!open)
            {
                entity.RemoveComponent<T>();
            }
        }
        else
        {
            if (ImGui::CollapsingHeader(header.c_str(), ImGuiTreeNodeFlags_DefaultOpen))
            {
                ImGui::PushID(header.c_str());
                uiFunction(entity.GetComponent<T>());
                ImGui::PopID();
            }
        }
    }
}

void MCEditor::InspectorPanel::OnImGuiRender() const
{
    ENGINE_PROFILE_FUNCTION();

    ImGui::Begin("Inspector");

    auto &&selectedEntity = SceneManager::GetInstance().GetSelectedEntity();
    if (selectedEntity)
    {
        // TagComponent
        DrawComponent<MCEngine::TagComponent>(
            "Tag Component", selectedEntity,
            [](MCEngine::TagComponent *tag) {
                DrawTable2<MCEngine::TagComponent>("Tag", [&tag]() {
                    char buffer[256];
                    memset(buffer, 0, sizeof(buffer));
                    strncpy(buffer, tag->Tag.c_str(), sizeof(buffer) - 1);
                    if (ImGui::InputText("##Tag", buffer, sizeof(buffer)))
                    {
                        tag->Tag = std::string(buffer);
                    }
                });
            },
            false);

        // TransformComponent
        DrawComponent<MCEngine::TransformComponent>(
            "Transform Component", selectedEntity, [](MCEngine::TransformComponent *transform) {
                DrawVec3Control(
                    "Position", transform->Position, [&](const glm::vec3 &value) { transform->Position = value; },
                    0.0f);
                DrawVec3Control(
                    "Rotation", transform->GetRotationEuler(),
                    [&](const glm::vec3 &value) { transform->SetRotationEuler(value); }, 0.0f);
                DrawVec3Control(
                    "Scale", transform->Scale, [&](const glm::vec3 &value) { transform->Scale = value; }, 1.0f);
            });

        // CameraComponent
        DrawComponent<MCEngine::CameraComponent>(
            "Camera Component", selectedEntity, [](MCEngine::CameraComponent *camera) {
                DrawTable2<MCEngine::CameraComponent>("Type", [&camera]() {
                    const char *cameraTypes[] = {"Orthographic", "Perspective"};
                    int currentType = static_cast<int>(camera->Type);
                    if (ImGui::Combo("##Camera Type", &currentType, cameraTypes, IM_ARRAYSIZE(cameraTypes)))
                    {
                        camera->Type = static_cast<MCEngine::CameraType>(currentType);
                    }
                });

                if (camera->Type == MCEngine::CameraType::Orthographic)
                {
                    DrawTable2<MCEngine::CameraComponent>(
                        "Scale", [&camera]() { ImGui::DragFloat("##Scale", &camera->Scale, 1.0f, 0.1f, 10.0f); });
                }

                if (camera->Type == MCEngine::CameraType::Perspective)
                {
                    DrawTable2<MCEngine::CameraComponent>(
                        "FOV", [&camera]() { ImGui::DragFloat("##FOV", &camera->FOV, 1.0f, 1.0f, 120.0f); });
                    DrawTable2<MCEngine::CameraComponent>("Near Clip", [&camera]() {
                        ImGui::DragFloat("##Near Clip", &camera->NearClip, 0.1f, 0.01f, 100.0f);
                    });
                    DrawTable2<MCEngine::CameraComponent>("Far Clip", [&camera]() {
                        ImGui::DragFloat("##Far Clip", &camera->FarClip, 1.0f, 10.0f, 1000.0f);
                    });
                }
            });

        // SpriteRendererComponent
        DrawComponent<MCEngine::SpriteRendererComponent>(
            "Sprite Renderer Component", selectedEntity, [](MCEngine::SpriteRendererComponent *sprite) {
                DrawTable2<MCEngine::SpriteRendererComponent>(
                    "Color", [&sprite]() { ImGui::ColorEdit4("##Color", glm::value_ptr(sprite->Color)); });
            });

        // todo: Drag and Drop for Texture2D
        if (ImGui::BeginDragDropTarget())
        {
            if (const ImGuiPayload *payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
            {
                if (payload->Data)
                {
                    const char *path = static_cast<const char *>(payload->Data);
                    std::filesystem::path filepath(path);
                    std::string relativePath =
                        std::filesystem::relative(filepath, ConfigManager::GetInstance().GetAssetsPath()).string();
                    if (std::filesystem::is_regular_file(filepath))
                    {
                        if (ConfigManager::IsTexture(filepath))
                        {
                            if (selectedEntity)
                            {
                                selectedEntity.GetComponent<MCEngine::SpriteRendererComponent>()->TextureInstance =
                                    MCEngine::TextureLibrary::GetInstance().GetTexture2D(relativePath);
                            }
                        }
                    }
                }
            }
            ImGui::EndDragDropTarget();
        }

        // MeshRendererComponent
        DrawComponent<MCEngine::MeshRendererComponent>(
            "Mesh Renderer Component", selectedEntity, [](MCEngine::MeshRendererComponent *meshRenderer) {
                DrawTable2<MCEngine::MeshRendererComponent>("Color", [&meshRenderer]() {
                    ImGui::ColorEdit4("##Color", glm::value_ptr(meshRenderer->MaterialInstance.Color));
                });
                DrawTable2<MCEngine::MeshRendererComponent>("Ambient", [&meshRenderer]() {
                    ImGui::DragFloat("##Ambient", &meshRenderer->MaterialInstance.AmbientStrength, 0.1f, 0.0f, 1.0f);
                });
                DrawTable2<MCEngine::MeshRendererComponent>("Diffuse", [&meshRenderer]() {
                    ImGui::DragFloat("##Diffuse", &meshRenderer->MaterialInstance.DiffuseStrength, 0.1f, 0.0f, 1.0f);
                });
                DrawTable2<MCEngine::MeshRendererComponent>("Specular", [&meshRenderer]() {
                    ImGui::DragFloat("##Specular", &meshRenderer->MaterialInstance.SpecularStrength, 0.1f, 0.0f, 1.0f);
                });
                DrawTable2<MCEngine::MeshRendererComponent>("Shininess", [&meshRenderer]() {
                    ImGui::DragFloat("##Shininess", &meshRenderer->MaterialInstance.Shininess, 1.0f, 1.0f, 256.0f);
                });
            });

        // LightComponent
        DrawComponent<MCEngine::LightComponent>("Light Component", selectedEntity, [](MCEngine::LightComponent *light) {
            DrawTable2<MCEngine::LightComponent>(
                "Color", [&light]() { ImGui::ColorEdit3("##Color", glm::value_ptr(light->Color)); });
            DrawTable2<MCEngine::LightComponent>(
                "Intensity", [&light]() { ImGui::DragFloat("##Intensity", &light->Intensity, 0.1f, 0.0f, 10.0f); });

            if (light->GetType() == MCEngine::LightType::Point || light->GetType() == MCEngine::LightType::Spot)
            {
                DrawTable2<MCEngine::LightComponent>(
                    "Constant", [&light]() { ImGui::DragFloat("##Constant", &light->Constant, 0.01f, 0.0f, 1.0f); });
                DrawTable2<MCEngine::LightComponent>(
                    "Linear", [&light]() { ImGui::DragFloat("##Linear", &light->Linear, 0.001f, 0.0f, 1.0f); });
                DrawTable2<MCEngine::LightComponent>("Quadratic", [&light]() {
                    ImGui::DragFloat("##Quadratic", &light->Quadratic, 0.0001f, 0.0f, 1.0f);
                });

                if (light->GetType() == MCEngine::LightType::Spot)
                {
                    DrawTable2<MCEngine::LightComponent>(
                        "CutOff", [&light]() { ImGui::DragFloat("##CutOff", &light->InnerAngle, 1.0f, 0.0f, 90.0f); });
                    DrawTable2<MCEngine::LightComponent>("Outer CutOff", [&light]() {
                        ImGui::DragFloat("##Outer CutOff", &light->OuterAngle, 1.0f, 0.0f, 90.0f);
                    });
                }
            }
        });

        // SkyboxComponent
        DrawComponent<MCEngine::SkyboxComponent>(
            "Skybox Component", selectedEntity, [](MCEngine::SkyboxComponent *skybox) {
                DrawTable2<MCEngine::SkyboxComponent>("Texture", [&skybox]() {
                    char buffer[256];
                    memset(buffer, 0, sizeof(buffer));
                    strncpy(buffer, skybox->TextureCubeName.c_str(), sizeof(buffer) - 1);
                    if (ImGui::InputText("##Texture", buffer, sizeof(buffer)))
                    {
                        skybox->TextureCubeName = std::string(buffer);
                    }
                });
            });

        DrawAddComponentButton(selectedEntity);
    }

    ImGui::End();
}

void MCEditor::InspectorPanel::DrawVec3Control(const std::string &label, const glm::vec3 &constValues,
                                               std::function<void(const glm::vec3 &)> callback, float resetValue)
{
    ENGINE_PROFILE_FUNCTION();

    ImGui::PushID(label.c_str());

    ImGui::BeginTable("Vec3Table", 4, ImGuiTableFlags_SizingStretchProp | ImGuiTableFlags_NoPadOuterX);
    ImGui::TableSetupColumn(label.c_str(), ImGuiTableColumnFlags_None, 0.16f);
    ImGui::TableSetupColumn("X", ImGuiTableColumnFlags_None, 0.28f);
    ImGui::TableSetupColumn("Y", ImGuiTableColumnFlags_None, 0.28f);
    ImGui::TableSetupColumn("Z", ImGuiTableColumnFlags_None, 0.28f);
    ImGui::TableNextRow();

    // Label
    ImGui::TableSetColumnIndex(0);
    ImGui::TextUnformatted(label.c_str());

    float lineHeight = ImGui::GetFontSize() + ImGui::GetStyle().FramePadding.y * 2.0f;
    ImVec2 buttonSize = {lineHeight + 3.0f, lineHeight};

    glm::vec3 values = constValues;

    // X
    ImGui::TableSetColumnIndex(1);
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.8f, 0.1f, 0.15f, 1.0f});
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.9f, 0.2f, 0.2f, 1.0f});
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.8f, 0.1f, 0.15f, 1.0f});
    if (ImGui::Button("X", buttonSize))
        values.x = resetValue;
    ImGui::PopStyleColor(3);
    ImGui::SameLine();
    ImGui::SetNextItemWidth(-FLT_MIN);
    ImGui::DragFloat("##X", &values.x, 0.1f);

    // Y
    ImGui::TableSetColumnIndex(2);
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.2f, 0.7f, 0.2f, 1.0f});
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.3f, 0.8f, 0.3f, 1.0f});
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.2f, 0.7f, 0.2f, 1.0f});
    if (ImGui::Button("Y", buttonSize))
        values.y = resetValue;
    ImGui::PopStyleColor(3);
    ImGui::SameLine();
    ImGui::SetNextItemWidth(-FLT_MIN);
    ImGui::DragFloat("##Y", &values.y, 0.1f);

    // Z
    ImGui::TableSetColumnIndex(3);
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.1f, 0.25f, 0.8f, 1.0f});
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.2f, 0.35f, 0.9f, 1.0f});
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.1f, 0.25f, 0.8f, 1.0f});
    if (ImGui::Button("Z", buttonSize))
        values.z = resetValue;
    ImGui::PopStyleColor(3);
    ImGui::SameLine();
    ImGui::SetNextItemWidth(-FLT_MIN);
    ImGui::DragFloat("##Z", &values.z, 0.1f);

    if (values != constValues)
        callback(values);

    ImGui::EndTable();

    ImGui::PopID();
}

// clang-format off
template <typename T>
// clang-format on
static void DisplayAddComponentEntry(const std::string &entryName)
{
    ENGINE_PROFILE_FUNCTION();

    auto &&selectionContext = MCEditor::SceneManager::GetInstance().GetSelectedEntity();
    if (!selectionContext.HasComponent<T>())
    {
        if (ImGui::MenuItem(entryName.c_str()))
        {
            selectionContext.AddComponent<T>();
            ImGui::CloseCurrentPopup();
        }
    }
}

void MCEditor::InspectorPanel::DrawAddComponentButton(MCEngine::Entity entity)
{
    ENGINE_PROFILE_FUNCTION();

    ImGui::Separator();

    if (ImGui::Button("Add Component", ImVec2(ImGui::GetContentRegionAvail().x, 0)))
        ImGui::OpenPopup("AddComponent");

    if (ImGui::BeginPopup("AddComponent"))
    {
        DisplayAddComponentEntry<MCEngine::SpriteRendererComponent>("Sprite Renderer Component");
        DisplayAddComponentEntry<MCEngine::MeshRendererComponent>("Mesh Renderer Component");

        ImGui::Separator();

        DisplayAddComponentEntry<MCEngine::CameraComponent>("Camera Component");

        if (!entity.HasComponent<MCEngine::LightComponent>())
        {
            if (ImGui::MenuItem("Directional Light Component"))
            {
                entity.AddComponent<MCEngine::LightComponent>(MCEngine::LightType::Directional);
                ImGui::CloseCurrentPopup();
            }
        }

        if (!entity.HasComponent<MCEngine::LightComponent>())
        {
            if (ImGui::MenuItem("Point Light Component"))
            {
                entity.AddComponent<MCEngine::LightComponent>(MCEngine::LightType::Point);
                ImGui::CloseCurrentPopup();
            }
        }

        if (!entity.HasComponent<MCEngine::LightComponent>())
        {
            if (ImGui::MenuItem("Spot Light Component"))
            {
                entity.AddComponent<MCEngine::LightComponent>(MCEngine::LightType::Spot);
                ImGui::CloseCurrentPopup();
            }
        }

        DisplayAddComponentEntry<MCEngine::SkyboxComponent>("Skybox Component");

        ImGui::EndPopup();
    }
}