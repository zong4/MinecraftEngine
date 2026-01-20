#include "CreatorLayer.hpp"

#include "Scenes/EditorScene.hpp"
#include "Scenes/ExampleScene.hpp"
#include "Scripts/RotatingLight.hpp"
#include <imgui.h>

Editor::CreatorLayer::CreatorLayer() : Engine::Layer("CreatorLayer")
{
    m_EditorScene = std::make_shared<Editor::EditorScene>();
    m_ActiveScene = std::make_shared<Editor::ExampleScene>();
}

void Editor::CreatorLayer::OnEvent(Engine::Event &event)
{
    PROFILE_FUNCTION();

    Engine::EventDispatcher dispatcher(event);
    dispatcher.Dispatch<Engine::WindowResizeEvent>([this](Engine::WindowResizeEvent &event) {
        m_FbWidth = event.GetFbWidth();
        m_FbHeight = event.GetFbHeight();
        m_EditorScene->Resize(m_FbWidth, m_FbHeight);
        m_ActiveScene->Resize(m_FbWidth, m_FbHeight);
        return true;
    });
}

void Editor::CreatorLayer::OnUpdate(float deltaTime)
{
    PROFILE_FUNCTION();

    if (Engine::Input::GetInstance().IsKeyReleased(KEY_SPACE))
    {
        m_Mode = (m_Mode == SandboxMode::Edit) ? SandboxMode::Play : SandboxMode::Edit;
    }

    if (Engine::Input::GetInstance().IsKeyReleased(KEY_R))
    {
        if (m_Mode == SandboxMode::Edit)
        {
            m_ActiveScene = std::make_shared<Editor::ExampleScene>();
            m_ActiveScene->Resize(m_FbWidth, m_FbHeight);
        }
    }

    switch (m_Mode)
    {
    case SandboxMode::Edit:
        m_EditorScene->Update(deltaTime);
        m_EditorScene->UpdateRuntime(deltaTime);
        m_ActiveScene->Update(deltaTime);
        break;
    case SandboxMode::Play:
        m_ActiveScene->Update(deltaTime);
        m_ActiveScene->UpdateRuntime(deltaTime);
        break;
    default:
        LOG_ENGINE_ASSERT("Unknown SandboxMode!");
        break;
    }
}

void Editor::CreatorLayer::OnRender()
{
    PROFILE_FUNCTION();

    switch (m_Mode)
    {
    case SandboxMode::Edit:
        m_ActiveScene->Render(m_EditorScene->GetMainCamera());
        break;
    case SandboxMode::Play:
        m_ActiveScene->Render(m_ActiveScene->GetMainCamera());
        break;
    default:
        LOG_ENGINE_ASSERT("Unknown SandboxMode!");
        break;
    }
}

void Editor::CreatorLayer::OnImGuiRender()
{
    PROFILE_FUNCTION();

    DrawDebugUI();
    DrawConsole(Engine::g_Console);
}

void Editor::CreatorLayer::DrawDebugUI()
{
    PROFILE_FUNCTION();

    ImGui::Begin("Debug Info");

    // Sandbox Mode
    if (m_Mode == SandboxMode::Edit)
        ImGui::Text("Mode: Edit (Press SPACE to Switch to Play)");
    else
        ImGui::Text("Mode: Play (Press SPACE to Switch to Edit)");
    ImGui::Text("Press R to Reset Scene in Edit Mode");

    ImGuiIO &io = ImGui::GetIO();
    bool mouseBlock = io.WantCaptureMouse && ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow);
    bool keyBlock = io.WantCaptureKeyboard && ImGui::IsAnyItemActive();
    Engine::Input::GetInstance().SetBlockEvents(mouseBlock || keyBlock);
    ImGui::Text("ImGui Blocking Events: %s", (mouseBlock || keyBlock) ? "True" : "False");

    auto &&drawLabeledControl = [](const char *label, const std::function<void()> &controlFn) {
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
                ImGui::ColorEdit4("##CubeColor",
                                  glm::value_ptr(material->GetProperty("Color").GetValueAs<glm::vec4>()));
            });
            drawLabeledControl("Ambient", [&]() {
                ImGui::SliderFloat("##CubeAmbient", &material->GetProperty("AmbientStrength").GetValueAs<float>(), 0.0f,
                                   1.0f);
            });
            drawLabeledControl("Diffuse", [&]() {
                ImGui::SliderFloat("##CubeDiffuse", &material->GetProperty("DiffuseStrength").GetValueAs<float>(), 0.0f,
                                   1.0f);
            });
            drawLabeledControl("Specular", [&]() {
                ImGui::SliderFloat("##CubeSpecular", &material->GetProperty("SpecularStrength").GetValueAs<float>(),
                                   0.0f, 1.0f);
            });
            drawLabeledControl("Shininess", [&]() {
                ImGui::SliderFloat("##CubeShininess", &material->GetProperty("Shininess").GetValueAs<float>(), 1.0f,
                                   256.0f);
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

            drawLabeledControl("Color", [&]() { ImGui::ColorEdit4("##LightColor", glm::value_ptr(lightComp->Color)); });
            drawLabeledControl("Intensity",
                               [&]() { ImGui::SliderFloat("##LightIntensity", &lightComp->Intensity, 0.0f, 10.0f); });

            bool rotateLight = Editor::RotatingLight::IsRotationEnabled();
            drawLabeledControl("Rotate", [&]() {
                if (ImGui::Checkbox("##RotateLight", &rotateLight))
                    Editor::RotatingLight::SetRotationEnabled(rotateLight);
            });

            ImGui::EndTable();
        }
    }

    ImGui::End();
}

void Editor::CreatorLayer::DrawConsole(Engine::Console &console)
{
    PROFILE_FUNCTION();

    ImGui::Begin("Console");

    // Console output area
    ImGui::BeginChild("ScrollingRegion", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()), false);
    // Display console items
    for (const auto &item : console.Items)
        ImGui::TextUnformatted(item.c_str());
    // Auto-scroll to bottom
    if (console.ScrollToBottom)
    {
        ImGui::SetScrollHereY(1.0f);
        console.ScrollToBottom = false;
    }
    ImGui::EndChild();

    // Input text box
    ImGui::Separator();
    ImGui::SetNextItemWidth(-FLT_MIN);
    if (ImGui::InputText("##Input", console.InputBuf, IM_ARRAYSIZE(console.InputBuf),
                         ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll))
    {
        std::string inputStr = console.InputBuf;
        if (!inputStr.empty())
        {
            Engine::CommandManager::GetInstance().Execute(inputStr);
            console.InputBuf[0] = '\0';
        }
    }

    ImGui::End();
}