#include "OpenGLImGuiLayer.hpp"

#include "../Renderers/RendererProperty.hpp"
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

void Engine::OpenGLImGuiLayer::InitRenderer()
{
    ImGui_ImplGlfw_InitForOpenGL(static_cast<GLFWwindow *>(m_NativeWindow), true);
    std::string glsl_version = "#version " + std::to_string(Engine::RendererProperty::GetInstance().GetMajorVersion()) +
                               std::to_string(Engine::RendererProperty::GetInstance().GetMinorVersion()) + "0";
    ImGui_ImplOpenGL3_Init(glsl_version.c_str());
}

void Engine::OpenGLImGuiLayer::BeginRenderImGui() const
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void Engine::OpenGLImGuiLayer::EndRenderImGui() const
{
    int width, height;
    glfwGetWindowSize(static_cast<GLFWwindow *>(m_NativeWindow), &width, &height);
    ImGuiIO &io = ImGui::GetIO();
    io.DisplaySize = ImVec2((float)width, (float)height);
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Engine::OpenGLImGuiLayer::ShutdownRenderer() { ImGui_ImplOpenGL3_Shutdown(); }