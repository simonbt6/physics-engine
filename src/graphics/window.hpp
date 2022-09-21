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
        inline GUI* getGUI() const { return m_GUI; }

        void onAttach();
        void onDetach();

        void onUpdate();
        void onEvent();

        void setFullScreen(bool value);

        void PushLayer(Layer* layer);

    private:
        static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
    };
}
