#include "InspectorPanel.hpp"

#include "../Scene/ScenesManager.hpp"
#include <imgui.h>
#include <imgui_internal.h>

Editor::InspectorPanel &Editor::InspectorPanel::GetInstance()
{
    static InspectorPanel instance;
    return instance;
}

// clang-format off
template <typename T, typename UIFunction>
// clang-format on
static void DrawTable2(const std::string &label, UIFunction &&uiFunction)
{
    PROFILE_FUNCTION();

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
static void DrawComponent(const std::string &name, Engine::Entity entity, UIFunction &&uiFunction,
                          bool removeable = true)
{
    PROFILE_FUNCTION();

    if (entity.HasComponent<T>())
    {
        std::string header = name + "##" + std::to_string(static_cast<uint32_t>(entity));
        if (removeable) // Removeable component with close button
        {
            bool open = true;
            if (ImGui::CollapsingHeader(header.c_str(), &open, ImGuiTreeNodeFlags_DefaultOpen))
            {
                ImGui::PushID(header.c_str());
                uiFunction(entity.GetComponent<T>());
                ImGui::PopID();
            }
            if (!open)
                entity.RemoveComponent<T>();
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

void Editor::InspectorPanel::OnImGuiRender() const
{
    PROFILE_FUNCTION();

    ImGui::Begin("Inspector");
    auto &&selectedEntity = ScenesManager::GetInstance().GetSelectedEntity();
    if (selectedEntity)
    {
        // LabelComponent
        DrawComponent<Engine::LabelComponent>(
            "Label Component", selectedEntity,
            [](Engine::LabelComponent *label) {
                DrawTable2<Engine::LabelComponent>("Tag", [&label]() {
                    const char *tagTypes[] = {"None", "Player", "MainCamera", "Light", "Skybox"};
                    int currentTag = static_cast<int>(label->Tag);
                    if (ImGui::Combo("##Tag", &currentTag, tagTypes, IM_ARRAYSIZE(tagTypes)))
                    {
                        label->Tag = static_cast<Engine::TagType>(currentTag);
                    }
                });
                DrawTable2<Engine::LabelComponent>("Layer", [&label]() {
                    const char *layerTypes[] = {"Default", "UI", "Background", "Foreground"};
                    int currentLayer = static_cast<int>(label->Layer);
                    if (ImGui::Combo("##Layer", &currentLayer, layerTypes, IM_ARRAYSIZE(layerTypes)))
                    {
                        label->Layer = static_cast<Engine::LayerType>(currentLayer);
                    }
                });
                DrawTable2<Engine::LabelComponent>("Name", [&label]() { InputTextString("##Name", &label->Name); });
            },
            false);

        // TransformComponent
        DrawComponent<Engine::TransformComponent>("Transform Component", selectedEntity,
                                                  [](Engine::TransformComponent *transform) {
                                                      DrawVec3Control("Position", transform->Position, 0.0f);
                                                      glm::vec3 rotation = transform->GetRotationEuler();
                                                      DrawVec3Control("Rotation", rotation, 0.0f);
                                                      transform->SetRotationEuler(rotation);
                                                      DrawVec3Control("Scale", transform->Scale, 1.0f);
                                                  });

        // CameraComponent
        DrawComponent<Engine::CameraComponent>(
            "Camera Component", selectedEntity, [&selectedEntity](Engine::CameraComponent *camera) {
                // Common
                {
                    DrawTable2<Engine::CameraComponent>("Type", [&camera]() {
                        const char *cameraTypes[] = {"Orthographic", "Perspective"};
                        int currentType = static_cast<int>(camera->Type);
                        if (ImGui::Combo("##Camera Type", &currentType, cameraTypes, IM_ARRAYSIZE(cameraTypes)))
                        {
                            camera->Type = static_cast<Engine::CameraType>(currentType);
                        }
                    });
                    DrawTable2<Engine::CameraComponent>("Primary", [&camera, &selectedEntity]() {
                        bool primary = camera->Primary;
                        if (ImGui::Checkbox("##Primary", &primary))
                        {
                            ScenesManager::GetInstance().GetActiveScene()->SetMainCamera(selectedEntity);
                        }
                    });
                    DrawTable2<Engine::CameraComponent>("Background Color", [&camera]() {
                        ImGui::ColorEdit4("##Background Color", glm::value_ptr(camera->BackgroundColor));
                    });
                }

                // Orthographic
                if (camera->Type == Engine::CameraType::Orthographic)
                {
                    DrawTable2<Engine::CameraComponent>(
                        "Scale", [&camera]() { ImGui::DragFloat("##Scale", &camera->Scale, 1.0f, 0.1f, 10.0f); });
                    DrawTable2<Engine::CameraComponent>("Near Clip", [&camera]() {
                        ImGui::DragFloat("##Near Clip", &camera->NearClip, 0.1f, 0.01f, 100.0f);
                    });
                    DrawTable2<Engine::CameraComponent>("Far Clip", [&camera]() {
                        ImGui::DragFloat("##Far Clip", &camera->FarClip, 1.0f, 10.0f, 1000.0f);
                    });
                }

                // Perspective
                if (camera->Type == Engine::CameraType::Perspective)
                {
                    DrawTable2<Engine::CameraComponent>(
                        "FOV", [&camera]() { ImGui::DragFloat("##FOV", &camera->FOV, 1.0f, 1.0f, 120.0f); });
                    DrawTable2<Engine::CameraComponent>("Near Clip", [&camera]() {
                        ImGui::DragFloat("##Near Clip", &camera->NearClip, 0.1f, 0.01f, 100.0f);
                    });
                    DrawTable2<Engine::CameraComponent>("Far Clip", [&camera]() {
                        ImGui::DragFloat("##Far Clip", &camera->FarClip, 1.0f, 10.0f, 1000.0f);
                    });
                }
            });

        // MaterialComponent
        DrawComponent<Engine::MaterialComponent>(
            "Material Component", selectedEntity, [](Engine::MaterialComponent *materialComp) {
                // Material name
                std::string materialName = Engine::MaterialsManager::GetInstance().GetName(materialComp->GetMaterial());
                if (!materialName.empty())
                {
                    DrawTable2<Engine::MaterialComponent>("Material", [&materialName, &materialComp]() {
                        const char *materialNames[] = {"Default3D", "GrassBlock", "StoneBlock"};
                        int materialIndex =
                            std::distance(std::begin(materialNames),
                                          std::find(std::begin(materialNames), std::end(materialNames), materialName));
                        if (ImGui::Combo("##Material", &materialIndex, materialNames, IM_ARRAYSIZE(materialNames)))
                        {
                            std::shared_ptr<Engine::Material> newMaterial =
                                Engine::MaterialsManager::GetInstance().GetMaterial(materialNames[materialIndex]);
                            if (newMaterial)
                                materialComp->SetMaterial(newMaterial);
                        }

                        // Drag and drop
                        if (ImGui::BeginDragDropTarget())
                        {
                            if (const ImGuiPayload *payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
                            {
                                const char *path = (const char *)payload->Data;
                                std::filesystem::path filepath(path);

                                if (filepath.extension() == ".mcmat")
                                {
                                    auto newMaterial =
                                        Engine::MaterialsManager::GetInstance().GetMaterial(filepath.stem().string());
                                    if (newMaterial)
                                        materialComp->SetMaterial(newMaterial);
                                }
                            }
                            ImGui::EndDragDropTarget();
                        }
                    });
                }

                // Shader name
                if (auto &&shader = materialComp->GetMaterial()->GetShader())
                {
                    std::string shaderName = Engine::ShadersManager::GetInstance().GetName(shader);
                    DrawTable2<Engine::MaterialComponent>("Shader",
                                                          [&shaderName]() { ImGui::Text("%s", shaderName.c_str()); });
                }

                // Material properties
                for (auto &&[name, prop] : materialComp->GetPropertyMap())
                {
                    DrawTable2<Engine::MaterialComponent>(name, [&prop]() {
                        switch (prop.GetType())
                        {
                        case Engine::MaterialPropertyType::Float: {
                            float value = prop.GetValueAs<float>();
                            if (ImGui::DragFloat("##Value", &value))
                                prop.SetValue(value);
                            break;
                        }
                        case Engine::MaterialPropertyType::Vec3: {
                            glm::vec3 value = prop.GetValueAs<glm::vec3>();
                            if (ImGui::DragFloat3("##Value", glm::value_ptr(value)))
                                prop.SetValue(value);
                            break;
                        }
                        case Engine::MaterialPropertyType::Vec4: {
                            glm::vec4 value = prop.GetValueAs<glm::vec4>();
                            if (ImGui::ColorEdit4("##Value", glm::value_ptr(value)))
                                prop.SetValue(value);
                            break;
                        }
                        default:
                            ImGui::Text("Unsupported property type");
                            break;
                        }
                    });
                }
            });

        // LightComponent
        DrawComponent<Engine::LightComponent>("Light Component", selectedEntity, [](Engine::LightComponent *light) {
            // Common
            {
                DrawTable2<Engine::LightComponent>(
                    "Color", [&light]() { ImGui::ColorEdit3("##Color", glm::value_ptr(light->Color)); });
                DrawTable2<Engine::LightComponent>(
                    "Intensity", [&light]() { ImGui::DragFloat("##Intensity", &light->Intensity, 0.1f, 0.0f, 10.0f); });
            }

            // Point and Spot light only
            if (light->GetType() == Engine::LightType::Point || light->GetType() == Engine::LightType::Spot)
            {
                DrawTable2<Engine::LightComponent>(
                    "Constant", [&light]() { ImGui::DragFloat("##Constant", &light->Constant, 0.01f, 0.0f, 1.0f); });
                DrawTable2<Engine::LightComponent>(
                    "Linear", [&light]() { ImGui::DragFloat("##Linear", &light->Linear, 0.001f, 0.0f, 1.0f); });
                DrawTable2<Engine::LightComponent>("Quadratic", [&light]() {
                    ImGui::DragFloat("##Quadratic", &light->Quadratic, 0.0001f, 0.0f, 1.0f);
                });

                // Spot light only
                if (light->GetType() == Engine::LightType::Spot)
                {
                    DrawTable2<Engine::LightComponent>(
                        "CutOff", [&light]() { ImGui::DragFloat("##CutOff", &light->InnerAngle, 1.0f, 0.0f, 90.0f); });
                    DrawTable2<Engine::LightComponent>("Outer CutOff", [&light]() {
                        ImGui::DragFloat("##Outer CutOff", &light->OuterAngle, 1.0f, 0.0f, 90.0f);
                    });
                }
            }
        });

        // RigidBodyComponent
        DrawComponent<Engine::RigidBodyComponent>(
            "Rigid Body Component", selectedEntity, [](Engine::RigidBodyComponent *rigidBody) {
                DrawTable2<Engine::RigidBodyComponent>("Type", [&rigidBody]() {
                    const char *bodyTypes[] = {"Static", "Dynamic", "Kinematic"};
                    int currentType = static_cast<int>(rigidBody->Type);
                    if (ImGui::Combo("##RigidBody Type", &currentType, bodyTypes, IM_ARRAYSIZE(bodyTypes)))
                        rigidBody->Type = static_cast<Engine::RigidBodyType>(currentType);
                });
                DrawTable2<Engine::RigidBodyComponent>(
                    "Mass", [&rigidBody]() { ImGui::DragFloat("##Mass", &rigidBody->Mass, 10.0f, 0.0f, 100.0f); });
            });

        // AudioComponent
        DrawComponent<Engine::AudioComponent>("Audio Component", selectedEntity, [](Engine::AudioComponent *audio) {
            DrawTable2<Engine::AudioComponent>("Looping", [&audio]() {
                bool looping = audio->IsLooping();
                if (ImGui::Checkbox("##Looping", &looping))
                    audio->SetLooping(looping);
            });
            float volume = audio->GetVolume();
            DrawTable2<Engine::AudioComponent>("Volume", [&audio, &volume]() {
                ImGui::DragFloat("##Volume", &volume, 0.1f, 0.0f, 1.0f);
                audio->SetVolume(volume);
            });
        });

        // Add Component Button
        DrawAddComponentButton(selectedEntity);
    }
    ImGui::End();
}

bool Editor::InspectorPanel::InputTextString(const char *label, std::string *str)
{
    IM_ASSERT(str);
    char buf[256];
    strncpy(buf, str->c_str(), sizeof(buf) - 1);
    buf[sizeof(buf) - 1] = '\0';
    if (ImGui::InputText(label, buf, sizeof(buf)))
    {
        *str = buf;
        return true;
    }
    return false;
}

void Editor::InspectorPanel::DrawVec3Control(const std::string &label, glm::vec3 &values, float resetValue)
{
    PROFILE_FUNCTION();

    ImGui::PushID(label.c_str());

    // Begin Table
    ImGui::BeginTable("Vec3Table", 4, ImGuiTableFlags_SizingStretchProp | ImGuiTableFlags_NoPadOuterX);
    ImGui::TableSetupColumn(label.c_str(), ImGuiTableColumnFlags_None, 0.16f);
    ImGui::TableSetupColumn("X", ImGuiTableColumnFlags_None, 0.28f);
    ImGui::TableSetupColumn("Y", ImGuiTableColumnFlags_None, 0.28f);
    ImGui::TableSetupColumn("Z", ImGuiTableColumnFlags_None, 0.28f);
    ImGui::TableNextRow();

    // Label
    ImGui::TableSetColumnIndex(0);
    ImGui::TextUnformatted(label.c_str());

    // Calculate button size
    float lineHeight = ImGui::GetFontSize() + ImGui::GetStyle().FramePadding.y * 2.0f;
    ImVec2 buttonSize = {lineHeight + 3.0f, lineHeight};

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

    // End Table
    ImGui::EndTable();
    ImGui::PopID();
}

// clang-format off
template <typename T>
// clang-format on
static void DisplayAddComponentEntry(const std::string &entryName)
{
    PROFILE_FUNCTION();

    auto &&selectionContext = Editor::ScenesManager::GetInstance().GetSelectedEntity();
    if (!selectionContext.HasComponent<T>())
    {
        if (ImGui::MenuItem(entryName.c_str()))
        {
            selectionContext.AddComponent<T>();
            ImGui::CloseCurrentPopup();
        }
    }
}

void Editor::InspectorPanel::DrawAddComponentButton(Engine::Entity entity)
{
    PROFILE_FUNCTION();

    ImGui::Separator();

    if (ImGui::Button("Add Component", ImVec2(ImGui::GetContentRegionAvail().x, 0)))
        ImGui::OpenPopup("AddComponent");

    if (ImGui::BeginPopup("AddComponent"))
    {
        DisplayAddComponentEntry<Engine::MeshRendererComponent>("Mesh Renderer Component");

        DisplayAddComponentEntry<Engine::MaterialComponent>("Material Component");

        ImGui::Separator();

        DisplayAddComponentEntry<Engine::CameraComponent>("Camera Component");

        ImGui::Separator();

        if (!entity.HasComponent<Engine::LightComponent>())
        {
            if (ImGui::MenuItem("Directional Light Component"))
            {
                entity.AddComponent<Engine::LightComponent>(Engine::LightType::Directional);
                ImGui::CloseCurrentPopup();
            }
        }
        if (!entity.HasComponent<Engine::LightComponent>())
        {
            if (ImGui::MenuItem("Point Light Component"))
            {
                entity.AddComponent<Engine::LightComponent>(Engine::LightType::Point);
                ImGui::CloseCurrentPopup();
            }
        }
        if (!entity.HasComponent<Engine::LightComponent>())
        {
            if (ImGui::MenuItem("Spot Light Component"))
            {
                entity.AddComponent<Engine::LightComponent>(Engine::LightType::Spot);
                ImGui::CloseCurrentPopup();
            }
        }

        ImGui::Separator();

        DisplayAddComponentEntry<Engine::RigidBodyComponent>("Rigid Body Component");

        ImGui::EndPopup();
    }
}