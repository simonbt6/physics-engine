#include "gui_layer.hpp"

#include <GLFW/glfw3.h>

#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_glfw.h>
#include <imgui_internal.h>


#define GLSL_VERSION "#version 450"

using namespace Graphics;

void GUI::onAttach(GLFWwindow* window)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void) io;

    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    ImGui::StyleColorsDark();

    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(GLSL_VERSION);
}

void GUI::onDetach()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void GUI::dockWindow(const std::string& window, DockPosition position)
{
    m_DockQueue.insert({window, position});
}

void GUI::onUpdate()
{
    
}

void GUI::onEvent()
{
    
}

void GUI::_resetDocking()
{
    printf("\n RESET DOCKING \n");
    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGuiID dock_id = ImGui::GetID("DockSpace");

    ImGui::DockBuilderRemoveNode(dock_id);
    ImGui::DockBuilderAddNode(dock_id, ImGuiDockNodeFlags_PassthruCentralNode | ImGuiDockNodeFlags_DockSpace | ImGuiDockNodeFlags_NoDockingInCentralNode);
    ImGui::DockBuilderSetNodeSize(dock_id, viewport->Size);

    ImGuiID top_dock_id = ImGui::DockBuilderSplitNode(dock_id, ImGuiDir_Up, 0.2f, 0, &dock_id);
    ImGuiID bottom_dock_id = ImGui::DockBuilderSplitNode(dock_id, ImGuiDir_Down, 0.25f, 0, &dock_id);
    ImGuiID right_dock_id = ImGui::DockBuilderSplitNode(dock_id, ImGuiDir_Right, 0.15f, 0, &dock_id);
    ImGuiID left_dock_id = ImGui::DockBuilderSplitNode(dock_id, ImGuiDir_Left, 0.2f, 0, &dock_id);

    for (auto p : m_DockQueue)
    {
        switch (p.second)
        {
            case DockPosition::TOP:
                ImGui::DockBuilderDockWindow(p.first.c_str(), top_dock_id);
                break;
            case DockPosition::BOTTOM:
                ImGui::DockBuilderDockWindow(p.first.c_str(), bottom_dock_id);
                break;
            case DockPosition::LEFT:
                ImGui::DockBuilderDockWindow(p.first.c_str(), left_dock_id);
                break;
            case DockPosition::RIGHT:
                printf("\nDocking to the RIGHT %s\n", p.first.c_str());
                ImGui::DockBuilderDockWindow(p.first.c_str(), right_dock_id);
                break;
            case DockPosition::CENTER:
                ImGui::DockBuilderDockWindow(p.first.c_str(), dock_id);
                break;
        }
    }
    m_DockQueue.clear();
    
    ImGui::DockBuilderFinish(dock_id);
}

void GUI::begin()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGuiWindowFlags flags = ImGuiWindowFlags_MenuBar 
                           | ImGuiWindowFlags_NoDocking
                           | ImGuiWindowFlags_NoTitleBar
                           | ImGuiWindowFlags_NoCollapse
                           | ImGuiWindowFlags_NoResize
                           | ImGuiWindowFlags_NoMove
                           | ImGuiWindowFlags_NoBringToFrontOnFocus
                           | ImGuiWindowFlags_NoNavFocus
                           | ImGuiWindowFlags_NoBackground;

    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->Pos);
    ImGui::SetNextWindowSize(viewport->Size);
    ImGui::SetNextWindowViewport(viewport->ID);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

    ImGui::Begin("DockSpace", 0, flags);
    ImGui::PopStyleVar();
    ImGui::PopStyleVar(2);

    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
    {
        ImGuiID dock_id = ImGui::GetID("DockSpace");
        ImGui::DockSpace(dock_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);

        if (m_DockQueue.size() > 0)
            _resetDocking();
    }
    ImGui::End();    
    
}

void GUI::end()
{

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    
    if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        GLFWwindow* backup_ctx = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_ctx);
    }
}