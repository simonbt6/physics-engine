#include "context.hpp"

#include <backends/imgui_impl_opengl3.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

using namespace Graphics;

static ref<Context> s_Context = nullptr;

static void glfw_error_callback(int error, const char* desc) 
{
    fprintf(stderr, "GLFW error %d: %s\n", error, desc);
}

Context::Context()
{
    init();
}

Context::~Context()
{
    terminate();
}

void Context::init()
{
    glfwSetErrorCallback(glfw_error_callback);

    if (!glfwInit()) 
    {
        exit(EXIT_FAILURE);
    }
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, 1);
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    

}

void Context::loop()
{
    while (m_Windows.size() > 0)
    {
        for (Window* window : m_Windows)
        {
            if (window->IsRunning())
                window->onUpdate();
            else
                PopWindow(window);
        }    
    }
}

void Context::PushWindow(Window* window)
{
    m_Windows.push_back(window);
}

void Context::PopWindow(Window* window)
{
    auto it = std::find(m_Windows.begin(), m_Windows.end(), window);
    if (it != m_Windows.end())
        m_Windows.erase(it);
}

void Context::terminate()
{
    for (Window* window : m_Windows)
    {
        PopWindow(window);
        delete window;
    }
}

ref<Context> Context::GetInstance()
{
    if (s_Context == nullptr)
        s_Context.reset(new Context());

    return s_Context;
}