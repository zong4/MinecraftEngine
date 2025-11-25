#include "CreatorLayer.hpp"

#include "Scenes/EditorScene.hpp"
#include "Scenes/ExampleScene.hpp"
#include <imgui.h>
#include <cfloat>
#include <functional>

Editor::CreatorLayer::CreatorLayer(const std::shared_ptr<Engine::Window> &window)
    : Engine::Layer("CreatorLayer"), m_Window(window)
{
    m_EditorScene = std::make_shared<Editor::EditorScene>();
    m_ActiveScene = std::make_shared<Editor::ExampleScene>();
}

void Editor::CreatorLayer::OnUpdate(float deltaTime)
{
    PROFILE_FUNCTION();

    if (Engine::Input::GetInstance().IsKeyPressed(KEY_SPACE))
        m_Mode = SandboxMode::Play;
    else if (Engine::Input::GetInstance().IsKeyPressed(KEY_ESCAPE))
        m_Mode = SandboxMode::Edit;

    switch (m_Mode)
    {
    case SandboxMode::Edit:
        m_EditorScene->Update(deltaTime);
        m_ActiveScene->Update(deltaTime);
        break;
    case SandboxMode::Play:
        m_ActiveScene->Update(deltaTime);
        break;
    default:
        LOG_ENGINE_WARN("Unknown SandboxMode!");
        break;
    }
}

void Editor::CreatorLayer::OnRender()
{
    PROFILE_FUNCTION();

    // Resize scenes' viewport if the window size changed
    if (m_ViewportWidth != m_Window->GetProperty().FbWidth || m_ViewportHeight != m_Window->GetProperty().FbHeight)
    {
        m_ViewportWidth = m_Window->GetProperty().FbWidth;
        m_ViewportHeight = m_Window->GetProperty().FbHeight;
        m_EditorScene->Resize(m_ViewportWidth, m_ViewportHeight);
        m_ActiveScene->Resize(m_ViewportWidth, m_ViewportHeight);
    }

    switch (m_Mode)
    {
    case SandboxMode::Edit:
        m_ActiveScene->Render(m_EditorScene->GetMainCamera());
        break;
    case SandboxMode::Play:
        m_ActiveScene->Render(m_ActiveScene->GetMainCamera());
        break;
    default:
        LOG_ENGINE_WARN("Unknown SandboxMode!");
        break;
    }
}

void Editor::CreatorLayer::OnImGuiRender()
{
    PROFILE_FUNCTION();

    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.08f, 0.09f, 0.10f, 0.65f));
    ImGui::Begin("Sandbox Layer Debug");
    if (m_Mode == SandboxMode::Edit)
        ImGui::Text("Mode: Edit (Press SPACE to Play)");
    else
        ImGui::Text("Mode: Play (Press ESCAPE to Edit)");

    ImGuiIO &io = ImGui::GetIO();
    bool mouseBlock = io.WantCaptureMouse && ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow);
    bool keyBlock = io.WantCaptureKeyboard && ImGui::IsAnyItemActive();
    Engine::Input::GetInstance().SetBlockEvents(mouseBlock || keyBlock);
    ImGui::Text("ImGui Blocking Events: %s", (mouseBlock || keyBlock) ? "True" : "False");

    auto drawLabeledControl = [](const char *label, const std::function<void()> &controlFn) {
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::TextUnformatted(label);
        ImGui::TableSetColumnIndex(1);
        ImGui::SetNextItemWidth(-FLT_MIN);
        controlFn();
    };

    auto &&cube = m_ActiveScene->GetEntityByName("Cube");
    if (cube)
    {
        ImGui::SeparatorText("Cube");
        auto &&material = cube.GetComponent<Engine::MaterialComponent>();
        if (ImGui::BeginTable("CubeProperties", 2, ImGuiTableFlags_SizingStretchSame))
        {
            drawLabeledControl("Color", [&]() {
                ImGui::ColorEdit4("##CubeColor", glm::value_ptr(material->GetProperty("Color").GetValueAs<glm::vec4>()));
            });
            drawLabeledControl("Ambient", [&]() {
                ImGui::SliderFloat("##CubeAmbient", &material->GetProperty("AmbientStrength").GetValueAs<float>(), 0.0f, 1.0f);
            });
            drawLabeledControl("Diffuse", [&]() {
                ImGui::SliderFloat("##CubeDiffuse", &material->GetProperty("DiffuseStrength").GetValueAs<float>(), 0.0f, 1.0f);
            });
            drawLabeledControl("Specular", [&]() {
                ImGui::SliderFloat("##CubeSpecular", &material->GetProperty("SpecularStrength").GetValueAs<float>(), 0.0f, 1.0f);
            });
            drawLabeledControl("Shininess", [&]() {
                ImGui::SliderFloat("##CubeShininess", &material->GetProperty("Shininess").GetValueAs<float>(), 1.0f, 256.0f);
            });
            ImGui::EndTable();
        }
    }

    auto &&light = m_ActiveScene->GetEntityByName("DirectionalLight");
    if (light)
    {
        ImGui::SeparatorText("Directional Light");
        auto &&transform = light.GetComponent<Engine::TransformComponent>();
        auto &&lightComp = light.GetComponent<Engine::LightComponent>();

        if (ImGui::BeginTable("LightProperties", 2, ImGuiTableFlags_SizingStretchSame))
        {
            drawLabeledControl("Position", [&]() {
                ImGui::SliderFloat3("##LightPosition", glm::value_ptr(transform->Position), -10.0f, 10.0f);
            });

            glm::vec3 rotationEuler = transform->GetRotationEuler();
            drawLabeledControl("Rotation", [&]() {
                if (ImGui::SliderFloat3("##LightRotation", glm::value_ptr(rotationEuler), -180.0f, 180.0f))
                    transform->SetRotationEuler(rotationEuler);
            });

            drawLabeledControl("Color", [&]() {
                ImGui::ColorEdit4("##LightColor", glm::value_ptr(lightComp->Color));
            });

            drawLabeledControl("Intensity", [&]() {
                ImGui::SliderFloat("##LightIntensity", &lightComp->Intensity, 0.0f, 10.0f);
            });

            bool rotateLight = Editor::RotatingLight::IsRotationEnabled();
            drawLabeledControl("Rotate", [&]() {
                if (ImGui::Checkbox("##RotateLight", &rotateLight))
                    Editor::RotatingLight::SetRotationEnabled(rotateLight);
            });

            ImGui::EndTable();
        }
    }

    ImGui::End();
    ImGui::PopStyleColor();
}
