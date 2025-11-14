#include "InspectorPanel.hpp"

#include "Manager/ConfigManager.hpp"
#include "Manager/SceneManager.hpp"
#include "Renderer/Material/MaterialLibrary.hpp"
#include "Renderer/Shader/ShaderLibrary.hpp"
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

    if (entity.HasComponent<T>())
    {
        std::string header = name + "##" + std::to_string(static_cast<uint32_t>(entity));

        // Removeable component with close button
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
                DrawTable2<MCEngine::TagComponent>("Tag", [&tag]() { InputTextString("##Tag", &tag->Tag); });
            },
            false);

        // TransformComponent
        DrawComponent<MCEngine::TransformComponent>("Transform Component", selectedEntity,
                                                    [](MCEngine::TransformComponent *transform) {
                                                        DrawVec3Control("Position", transform->Position, 0.0f);
                                                        glm::vec3 rotation = transform->GetRotationEuler();
                                                        DrawVec3Control("Rotation", rotation, 0.0f);
                                                        transform->SetRotationEuler(rotation);
                                                        DrawVec3Control("Scale", transform->Scale, 1.0f);
                                                    });

        // CameraComponent
        DrawComponent<MCEngine::CameraComponent>(
            "Camera Component", selectedEntity, [&selectedEntity](MCEngine::CameraComponent *camera) {
                // Common
                {
                    DrawTable2<MCEngine::CameraComponent>("Type", [&camera]() {
                        const char *cameraTypes[] = {"Orthographic", "Perspective"};
                        int currentType = static_cast<int>(camera->Type);
                        if (ImGui::Combo("##Camera Type", &currentType, cameraTypes, IM_ARRAYSIZE(cameraTypes)))
                        {
                            camera->Type = static_cast<MCEngine::CameraType>(currentType);
                        }
                    });
                    DrawTable2<MCEngine::CameraComponent>("Primary", [&camera, &selectedEntity]() {
                        bool primary = camera->Primary;
                        if (ImGui::Checkbox("##Primary", &primary))
                        {
                            SceneManager::GetInstance().GetActiveScene()->SetMainCamera(selectedEntity);
                        }
                    });
                    DrawTable2<MCEngine::CameraComponent>("Background Color", [&camera]() {
                        ImGui::ColorEdit4("##Background Color", glm::value_ptr(camera->BackgroundColor));
                    });
                }

                // Orthographic
                if (camera->Type == MCEngine::CameraType::Orthographic)
                {
                    DrawTable2<MCEngine::CameraComponent>(
                        "Scale", [&camera]() { ImGui::DragFloat("##Scale", &camera->Scale, 1.0f, 0.1f, 10.0f); });
                    DrawTable2<MCEngine::CameraComponent>("Near Clip", [&camera]() {
                        ImGui::DragFloat("##Near Clip", &camera->NearClip, 0.1f, 0.01f, 100.0f);
                    });
                    DrawTable2<MCEngine::CameraComponent>("Far Clip", [&camera]() {
                        ImGui::DragFloat("##Far Clip", &camera->FarClip, 1.0f, 10.0f, 1000.0f);
                    });    
                }

                // Perspective
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

        // MaterialComponent (new system)
        DrawComponent<MCEngine::MaterialComponent>(
            "Material Component", selectedEntity, [](MCEngine::MaterialComponent *materialComp) {
                auto material = materialComp->GetMaterial();
                if (!material)
                {
                    ImGui::Text("No material assigned");
                    return;
                }

                // Display material name
                std::string materialName = MCEngine::MaterialLibrary::GetInstance().GetName(material);
                if (!materialName.empty())
                {
                    DrawTable2<MCEngine::MaterialComponent>("Material", [&materialName]() {
                        ImGui::Text("%s", materialName.c_str());
                    });
                }

                // Display shader name
                auto shader = material->GetShader();
                if (shader)
                {
                    std::string shaderName = MCEngine::ShaderLibrary::GetInstance().GetName(shader);
                    DrawTable2<MCEngine::MaterialComponent>("Shader", [&shaderName]() {
                        ImGui::Text("%s", shaderName.c_str());
                    });
                }

                // Display common material properties
                const auto &properties = material->GetProperties();
                
                // Color property
                if (material->HasProperty("Color"))
                {
                    auto colorProp = material->GetProperty("Color");
                    if (colorProp.GetType() == MCEngine::MaterialPropertyType::Vec4)
                    {
                        glm::vec4 color = colorProp.GetVec4();
                        DrawTable2<MCEngine::MaterialComponent>("Color", [&material, &color]() {
                            if (ImGui::ColorEdit4("##Color", glm::value_ptr(color)))
                            {
                                material->SetVec4("Color", color);
                            }
                        });
                    }
                }

                // Float properties (AmbientStrength, DiffuseStrength, etc.)
                std::vector<std::string> floatProps = {"AmbientStrength", "DiffuseStrength", "SpecularStrength", 
                                                        "Shininess", "Metallic", "Roughness", "AO"};
                for (const auto &propName : floatProps)
                {
                    if (material->HasProperty(propName))
                    {
                        float value = material->GetProperty(propName).GetFloat();
                        DrawTable2<MCEngine::MaterialComponent>(propName, [&material, &propName, &value]() {
                            float minVal = (propName == "Shininess") ? 1.0f : 0.0f;
                            float maxVal = (propName == "Shininess") ? 256.0f : 1.0f;
                            if (ImGui::DragFloat(("##" + propName).c_str(), &value, 0.01f, minVal, maxVal))
                            {
                                material->SetFloat(propName, value);
                            }
                        });
                    }
                }

                // Vec3 properties (Albedo, etc.)
                if (material->HasProperty("Albedo"))
                {
                    glm::vec3 albedo = material->GetProperty("Albedo").GetVec3();
                    DrawTable2<MCEngine::MaterialComponent>("Albedo", [&material, &albedo]() {
                        if (ImGui::ColorEdit3("##Albedo", glm::value_ptr(albedo)))
                        {
                            material->SetVec3("Albedo", albedo);
                        }
                    });
                }

                // Property Overrides section
                if (!materialComp->PropertyOverrides.empty())
                {
                    ImGui::Separator();
                    ImGui::Text("Property Overrides");
                    for (const auto &[name, prop] : materialComp->PropertyOverrides)
                    {
                        if (prop.GetType() == MCEngine::MaterialPropertyType::Float)
                        {
                            float value = prop.GetFloat();
                            DrawTable2<MCEngine::MaterialComponent>(name + " (Override)", [&materialComp, &name, &value]() {
                                if (ImGui::DragFloat(("##" + name).c_str(), &value, 0.01f))
                                {
                                    materialComp->SetOverrideFloat(name, value);
                                }
                            });
                        }
                    }
                }
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
        DrawComponent<MCEngine::SkyboxComponent>("Skybox Component", selectedEntity,
                                                 [](MCEngine::SkyboxComponent *skybox) {
                                                     DrawTable2<MCEngine::SkyboxComponent>("Texture", [&skybox]() {
                                                         InputTextString("##Texture", &skybox->TextureCubeName);
                                                     });
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

        // Add Component Button
        DrawAddComponentButton(selectedEntity);
    }

    ImGui::End();
}

bool MCEditor::InspectorPanel::InputTextString(const char *label, std::string *str)
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

void MCEditor::InspectorPanel::DrawVec3Control(const std::string &label, glm::vec3 &values, float resetValue)
{
    ENGINE_PROFILE_FUNCTION();

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
        DisplayAddComponentEntry<MCEngine::MaterialComponent>("Material Component");

        ImGui::Separator();

        DisplayAddComponentEntry<MCEngine::CameraComponent>("Camera Component");

        ImGui::Separator();

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

        ImGui::Separator();

        DisplayAddComponentEntry<MCEngine::SkyboxComponent>("Skybox Component");

        ImGui::EndPopup();
    }
}