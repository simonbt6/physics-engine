#pragma once

#include "../def.hpp"
#include "layer/layer_stack.hpp"
#include "layer/gui_layer.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Graphics 
{
    class Window 
    {
    private:
        GLFWwindow* m_Window;

        int m_Width = 1280, m_Height = 720;

        bool m_IsRunning;

        GUI* m_GUI;
        LayerStack m_LayerStack;

    public:
        Window(int width, int height, const std::string& title = "Default Window Title");
        Window();
        ~Window();

        void stop();

        inline bool IsRunning() const { return m_IsRunning; }

        void onAttach();
        void onDetach();

        void onUpdate();
        void onEvent();

        void PushLayer(Layer* layer);
    };
}
