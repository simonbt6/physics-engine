#include "window.hpp"

#include "context.hpp"
#include "render/renderer.hpp"

#include <imgui.h>
#include <thread>

namespace Graphics {

    Window::Window(int width, int height, const std::string& title)
        :m_Width(width), m_Height(width)
    {
        Window();
    }

    Window::Window()
        : m_IsRunning(false), m_GUI(new GUI()) 
    {
        Context::GetInstance();

        m_Window = glfwCreateWindow(m_Width, m_Height, "Test GLFW/GLAD", NULL, NULL);

        if (!m_Window) 
        {
            glfwTerminate();
            exit(EXIT_FAILURE);
        }

        glfwMakeContextCurrent(m_Window);
        gladLoadGL();
        glfwSwapInterval(1);

        glfwSetWindowUserPointer(m_Window, this);

        Render::Renderer::init();

        m_GUI->onAttach(m_Window);

        m_IsRunning = true;
    }

    Window::~Window()
    {
        stop();
    }

    void Window::stop()
    {
        glfwSetWindowShouldClose(m_Window, true);

        glfwDestroyWindow(m_Window);
        glfwTerminate();
    }

    void Window::onUpdate()
    {
        if (glfwWindowShouldClose(m_Window))
        {
            m_IsRunning = false;
            return;
        }


        glfwPollEvents();

        m_GUI->begin();

        m_GUI->onUpdate();
        for (Layer* layer : m_LayerStack)
            layer->onUpdate();

        glfwGetFramebufferSize(m_Window, &m_Width, &m_Height);
        glViewport(0, 0, m_Width, m_Height);
      
        m_GUI->end();

        glfwSwapBuffers(m_Window);
    }

    void Window::PushLayer(Layer* layer)
    {
        m_LayerStack.Push(layer);
        layer->onAttach(m_Window);
    }

}