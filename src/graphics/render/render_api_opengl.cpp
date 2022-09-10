#include "render_api.hpp"

#include <glad/glad.h>

using namespace Graphics::Render;

void OpenGL_MessageCallback(
    uint32_t source, 
    uint32_t type, 
    uint32_t id, 
    uint32_t severity, 
    int32_t length, 
    const char* message, 
    const void* user_param)
{
    switch (severity)
    {
        case GL_DEBUG_SEVERITY_HIGH:            
            printf("\n[HIGH] %s.", message);
            return;
        case GL_DEBUG_SEVERITY_MEDIUM:          
            printf("\n[MEDIUM] %s.", message);
            return;
        case GL_DEBUG_SEVERITY_LOW:             
            printf("\n[LOW] %s.", message);
            return;
        case GL_DEBUG_SEVERITY_NOTIFICATION:    
            printf("\n[NOTIFICATION] %s.", message);
            return;
        default:    
            printf("\n[DEFAULT] %s.", message);
            return;
    };
}

void RenderAPI::init()
{
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(OpenGL_MessageCallback, 0);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, 0, 0);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LINE_SMOOTH);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
}

void RenderAPI::clear()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void RenderAPI::setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
{
    glViewport(x, y, width, height);
}

void RenderAPI::setClearColor(const glm::vec4& color)
{
    glClearColor(color.r, color.g, color.b, color.a);
}

void RenderAPI::setLineWidth(float width)
{
    glLineWidth(width);
}

void RenderAPI::drawIndexed(const ref<VertexArray>& vao, uint32_t index_count)
{
    vao->bind();
    uint32_t count = index_count ? index_count : vao->getIndexBuffer()->getCount();
    glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, 0);
}

void RenderAPI::drawLines(const ref<VertexArray>& vao, uint32_t vertex_count)
{
    vao->bind();
    glDrawArrays(GL_LINES, 0, vertex_count);
}