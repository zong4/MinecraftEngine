#include "ImGuiLayer.hpp"

#include "Event/EventDispatcher.hpp"
#include "Event/KeyEvent.hpp"
#include "Event/MouseEvent.hpp"
#include "Library/Font/FontLibrary.hpp"
#include "Window/WindowUtility.hpp"
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

MCEngine::ImGuiLayer::ImGuiLayer(const std::shared_ptr<Window> &window, const std::string &filePath,
                                 const std::string &name)
    : Layer(name), m_Window(window), m_ImGuiFilePath(filePath)
{
}

void MCEngine::ImGuiLayer::OnEvent(Event &event)
{
    ENGINE_PROFILE_FUNCTION();

    if (m_BlockEvents)
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

void MCEngine::ImGuiLayer::OnImGuiRender()
{
    BeginRenderImGui();
    RenderImGui();
    EndRenderImGui();
}

void MCEngine::ImGuiLayer::BeginRenderImGui() const
{
    ENGINE_PROFILE_FUNCTION();

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void MCEngine::ImGuiLayer::EndRenderImGui() const
{
    ENGINE_PROFILE_FUNCTION();

    ImGuiIO &io = ImGui::GetIO();
    io.DisplaySize = ImVec2((float)m_Window->GetProperty().Width, (float)m_Window->GetProperty().Height);

    // Rendering
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    // Update and Render additional Platform Windows
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        GLFWwindow *backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }
}

void MCEngine::ImGuiLayer::OnAttach()
{
    ENGINE_PROFILE_FUNCTION();

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;     // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;   // Enable Multi-Viewport / Platform Windows
    // io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoTaskBarIcons;
    // io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoMerge;

    // Set font size based on DPI scale and screen resolution
    std::pair<int, int> screenResolution = WindowUtility::GetScreenResolution();
    LOG_ENGINE_INFO("Screen Resolution: " + std::to_string(screenResolution.first) + "x" +
                    std::to_string(screenResolution.second));
    float dpiScale = WindowUtility::GetDPIScale();
    LOG_ENGINE_INFO("DPI Scale: " + std::to_string(dpiScale));
    float fontSize = 10.0f * dpiScale * WindowUtility::GetScreenResolutionScale();

    // Set default font
    FontLibrary::GetInstance().Init(fontSize, 0.8f);
    ImFont *customFont = FontLibrary::GetInstance().GetFont("Cute");
    if (!customFont)
    {
        LOG_EDITOR_INFO("Failed to load custom font! Using default font instead.");
        customFont = io.FontDefault;
    }
    io.FontDefault = customFont;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    // ImGui::StyleColorsClassic();
    SetDarkThemeColors();

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular
    // ones.
    ImGuiStyle &style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // Read ini file
    ImGui::GetIO().IniFilename = m_ImGuiFilePath.c_str();
    ImGui::LoadIniSettingsFromDisk(m_ImGuiFilePath.c_str());

    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(static_cast<GLFWwindow *>(m_Window->GetNativeWindow()), true);
    std::string glsl_version = "#version " + std::to_string(m_Window->GetRendererAPIProperty().GetMajorVersion()) +
                               std::to_string(m_Window->GetRendererAPIProperty().GetMinorVersion()) + "0";
    ImGui_ImplOpenGL3_Init(glsl_version.c_str());
}

void MCEngine::ImGuiLayer::OnDetach()
{
    ENGINE_PROFILE_FUNCTION();

    // Save ini file
    ImGui::SaveIniSettingsToDisk(m_ImGuiFilePath.c_str());

    // Cleanup
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

void MCEngine::ImGuiLayer::SetDarkThemeColors()
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