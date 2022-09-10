#include "gui_layer.hpp"

#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_glfw.h>


#define GLSL_VERSION "#version 130"

using namespace Graphics;

void GUI::onAttach(GLFWwindow* window)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void) io;

    ImGui::StyleColorsDark();

    
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(GLSL_VERSION);
}

void GUI::onDetach()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void GUI::onUpdate()
{
    // if (show_demo_window)
    //         ImGui::ShowDemoWindow(&show_demo_window);

    //     {
    //         static float f = 0.0f;
    //         static int counter = 0;
    //         ImGui::Begin("Hello world");

    //         ImGui::Text("This is some useful text.");

    //         ImGui::End();
    //     }

    //     if (show_another_window)
    //     {
    //         ImGui::Begin("Another window", &show_another_window);
    //         ImGui::Text("Another window text.");
    //         ImGui::End();
    //     }
}

void GUI::onEvent()
{
    
}

void GUI::begin()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();\
}

void GUI::end()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}