#include "ImGuiLayer.hpp"

#include "../Renderer/RendererProperty.hpp"
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

Engine::ImGuiLayer::ImGuiLayer(void *nativeWindow) : Layer("ImGuiLayer"), m_NativeWindow(nativeWindow) {}

void Engine::ImGuiLayer::OnEvent(Event &event)
{
    PROFILE_FUNCTION();

    if (m_BlockEvents && m_NativeWindow)
    {
        EventDispatcher dispatcher(event);

        // Keyboard
        dispatcher.Dispatch<KeyEvent>([](KeyEvent &event) {
            ImGuiIO &io = ImGui::GetIO();
            io.AddKeyEvent(static_cast<ImGuiKey>(event.GetCode()), event.GetAction() == 1 || event.GetAction() == 2);
            return io.WantCaptureKeyboard;
        });

        // Mouse
        dispatcher.Dispatch<MouseMoveEvent>([](MouseMoveEvent &event) {
            ImGuiIO &io = ImGui::GetIO();
            io.AddMousePosEvent((float)event.GetX(), (float)event.GetY());
            return io.WantCaptureMouse;
        });
        dispatcher.Dispatch<MouseButtonEvent>([](MouseButtonEvent &event) {
            ImGuiIO &io = ImGui::GetIO();
            io.AddMouseButtonEvent(event.GetCode(), event.GetAction() == 1 || event.GetAction() == 2);
            return io.WantCaptureMouse;
        });
    }
}

void Engine::ImGuiLayer::OnAttach()
{
    PROFILE_FUNCTION();

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls
                                                          // io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoTaskBarIcons;
                                                          // io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoMerge;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    // ImGui::StyleColorsClassic();
    SetDarkThemeColors();

    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(static_cast<GLFWwindow *>(m_NativeWindow), true);
    std::string glsl_version = "#version " + std::to_string(Engine::RendererProperty::GetInstance().GetMajorVersion()) +
                               std::to_string(Engine::RendererProperty::GetInstance().GetMinorVersion()) + "0";
    ImGui_ImplOpenGL3_Init(glsl_version.c_str());
}

void Engine::ImGuiLayer::BeginRenderImGui() const
{
    PROFILE_FUNCTION();

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void Engine::ImGuiLayer::EndRenderImGui() const
{
    PROFILE_FUNCTION();

    int width, height;
    glfwGetWindowSize(static_cast<GLFWwindow *>(m_NativeWindow), &width, &height);
    ImGuiIO &io = ImGui::GetIO();
    io.DisplaySize = ImVec2((float)width, (float)height);
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Engine::ImGuiLayer::OnDetach()
{
    PROFILE_FUNCTION();

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

static ImVec4 ReverseGammaCorrection(ImVec4 &&color)
{
    color.x = pow(color.x, 2.2f);
    color.y = pow(color.y, 2.2f);
    color.z = pow(color.z, 2.2f);
    return color;
}

void Engine::ImGuiLayer::SetDarkThemeColors()
{
    auto &colors = ImGui::GetStyle().Colors;
    colors[ImGuiCol_WindowBg] = ReverseGammaCorrection(ImVec4{0.1f, 0.105f, 0.11f, 1.0f});

    // Headers
    colors[ImGuiCol_Header] = ReverseGammaCorrection(ImVec4{0.2f, 0.205f, 0.21f, 1.0f});
    colors[ImGuiCol_HeaderHovered] = ReverseGammaCorrection(ImVec4{0.3f, 0.305f, 0.31f, 1.0f});
    colors[ImGuiCol_HeaderActive] = ReverseGammaCorrection(ImVec4{0.15f, 0.1505f, 0.151f, 1.0f});

    // Buttons
    colors[ImGuiCol_Button] = ReverseGammaCorrection(ImVec4{0.2f, 0.205f, 0.21f, 1.0f});
    colors[ImGuiCol_ButtonHovered] = ReverseGammaCorrection(ImVec4{0.3f, 0.305f, 0.31f, 1.0f});
    colors[ImGuiCol_ButtonActive] = ReverseGammaCorrection(ImVec4{0.15f, 0.1505f, 0.151f, 1.0f});

    // Frame BG
    colors[ImGuiCol_FrameBg] = ReverseGammaCorrection(ImVec4{0.2f, 0.205f, 0.21f, 1.0f});
    colors[ImGuiCol_FrameBgHovered] = ReverseGammaCorrection(ImVec4{0.3f, 0.305f, 0.31f, 1.0f});
    colors[ImGuiCol_FrameBgActive] = ReverseGammaCorrection(ImVec4{0.15f, 0.1505f, 0.151f, 1.0f});

    // Tabs
    colors[ImGuiCol_Tab] = ReverseGammaCorrection(ImVec4{0.15f, 0.1505f, 0.151f, 1.0f});
    colors[ImGuiCol_TabHovered] = ReverseGammaCorrection(ImVec4{0.38f, 0.3805f, 0.381f, 1.0f});
    colors[ImGuiCol_TabActive] = ReverseGammaCorrection(ImVec4{0.28f, 0.2805f, 0.281f, 1.0f});
    colors[ImGuiCol_TabUnfocused] = ReverseGammaCorrection(ImVec4{0.15f, 0.1505f, 0.151f, 1.0f});
    colors[ImGuiCol_TabUnfocusedActive] = ReverseGammaCorrection(ImVec4{0.2f, 0.205f, 0.21f, 1.0f});

    // Title
    colors[ImGuiCol_TitleBg] = ReverseGammaCorrection(ImVec4{0.15f, 0.1505f, 0.151f, 1.0f});
    colors[ImGuiCol_TitleBgActive] = ReverseGammaCorrection(ImVec4{0.15f, 0.1505f, 0.151f, 1.0f});
    colors[ImGuiCol_TitleBgCollapsed] = ReverseGammaCorrection(ImVec4{0.15f, 0.1505f, 0.151f, 1.0f});

    // Popup
    colors[ImGuiCol_PopupBg] = ReverseGammaCorrection(ImVec4{0.2f, 0.205f, 0.21f, 1.0f});
    // colors[ImGuiCol_PopupBgHovered] = ReverseGammaCorrection(ImVec4{0.3f, 0.305f, 0.31f, 1.0f});
    // colors[ImGuiCol_PopupBgActive] = ReverseGammaCorrection(ImVec4{0.15f, 0.1505f, 0.151f, 1.0f});
}