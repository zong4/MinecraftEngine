#include "SandboxLayer.hpp"

#include "Scenes/EditorScene.hpp"
#include "Scenes/ExampleScene.hpp"
#include <imgui.h>

Editor::SandboxLayer::SandboxLayer(std::shared_ptr<Engine::Window> window)
    : Engine::Layer("SandboxLayer"), m_Window(window)
{
    m_EditorScene = std::make_shared<Editor::EditorScene>();
    m_ActiveScene = std::make_shared<Editor::ExampleScene>();
}

void Editor::SandboxLayer::OnEvent(Engine::Event &event)
{
    PROFILE_FUNCTION();

    if (!event.IsHandled())
    {
        Engine::EventDispatcher dispatcher(event);
        dispatcher.Dispatch<Engine::KeyEvent>(
            std::function<bool(Engine::KeyEvent &)>(std::bind(&SandboxLayer::OnKeyEvent, this, std::placeholders::_1)));
    }
}

void Editor::SandboxLayer::OnUpdate(float deltaTime)
{
    PROFILE_FUNCTION();

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

void Editor::SandboxLayer::OnRender()
{
    PROFILE_FUNCTION();

    // Resize scenes' viewport if the window size changed
    if (m_ViewportWidth != m_Window->GetProperty().FbWidth || m_ViewportHeight != m_Window->GetProperty().FbHeight)
    {
        m_ViewportWidth = m_Window->GetProperty().FbWidth;
        m_ViewportHeight = m_Window->GetProperty().FbHeight;
        m_EditorScene->Resize((float)m_ViewportWidth, (float)m_ViewportHeight);
        m_ActiveScene->Resize((float)m_ViewportWidth, (float)m_ViewportHeight);
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

void Editor::SandboxLayer::OnImGuiRender()
{
    PROFILE_FUNCTION();

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

    auto &&cube = m_ActiveScene->GetEntityByName("Cube");
    if (cube)
    {
        auto &&material = cube.GetComponent<Engine::MaterialComponent>();
        ImGui::ColorEdit4("Cube Color", glm::value_ptr(material->GetProperty("Color").GetValueAs<glm::vec4>()));
        ImGui::SliderFloat("Cube Ambient", &material->GetProperty("AmbientStrength").GetValueAs<float>(), 0.0f, 1.0f);
        ImGui::SliderFloat("Cube Diffuse", &material->GetProperty("DiffuseStrength").GetValueAs<float>(), 0.0f, 1.0f);
        ImGui::SliderFloat("Cube Specular", &material->GetProperty("SpecularStrength").GetValueAs<float>(), 0.0f, 1.0f);
        ImGui::SliderFloat("Cube Shininess", &material->GetProperty("Shininess").GetValueAs<float>(), 1.0f, 256.0f);
    }

    auto &&light = m_ActiveScene->GetEntityByName("DirectionalLight");
    if (light)
    {
        auto &&transform = light.GetComponent<Engine::TransformComponent>();
        ImGui::SliderFloat3("Light Position", glm::value_ptr(transform->Position), -10.0f, 10.0f);
        glm::vec3 rotationEuler = transform->GetRotationEuler();
        ImGui::SliderFloat3("Light Rotation", glm::value_ptr(rotationEuler), -180.0f, 180.0f);
        transform->SetRotationEuler(rotationEuler);

        auto &&lightComp = light.GetComponent<Engine::LightComponent>();
        ImGui::ColorEdit4("Light Color", glm::value_ptr(lightComp->Color));
        ImGui::SliderFloat("Light Intensity", &lightComp->Intensity, 0.0f, 10.0f);
    }

    ImGui::End();
}

bool Editor::SandboxLayer::OnKeyEvent(Engine::KeyEvent &event)
{
    if (Engine::Input::GetInstance().IsKeyPressed(KEY_SPACE))
    {
        m_Mode = SandboxMode::Play;
        return true;
    }
    else if (Engine::Input::GetInstance().IsKeyPressed(KEY_ESCAPE))
    {
        m_Mode = SandboxMode::Edit;
        return true;
    }

    return false;
}