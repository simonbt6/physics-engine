#include "sandbox_layer.hpp"

#include <imgui.h>

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

#include "../../src/video/decoder.hpp"

using namespace Graphics;

namespace Test
{
    static float vertices[6 * 4] = {
		 0.0f,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		50.0f,  0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
		50.0f, 50.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		 0.0f, 50.0f, 0.0f, 0.0f, 0.0f, 1.0f
	};

    static uint32_t indices[6] = { 0, 1, 2, 2, 3, 0 };

    static ref<Engine::Video::Decoder> decoder;


    void SandboxLayer::onAttach(GLFWwindow* window) 
    {
        m_Window = window;

        shader = CreateRef<Render::Shader>("test", "assets/shaders/");

        vao = CreateRef<Render::VertexArray>();
        vbo = CreateRef<Render::VertexBuffer>(vertices, sizeof(vertices));
        ibo = CreateRef<Render::IndexBuffer>(indices, sizeof(indices) / sizeof(uint32_t));

        vbo->setLayout({
            { Render::ShaderDatatype::Float3, "a_Position" },
            { Render::ShaderDatatype::Float3, "a_Color"}
        });
        vao->addVertexBuffer(vbo);
        vao->setIndexBuffer(ibo);

        decoder = CreateRef<Engine::Video::Decoder>("assets/videos/demo.mp4");

        Window* _window = (Window*)glfwGetWindowUserPointer(m_Window);
        _window->getGUI()->dockWindow("Object Transform", DockPosition::BOTTOM);
        _window->getGUI()->dockWindow("Projection settings", DockPosition::RIGHT);
        _window->getGUI()->dockWindow("Graphic view", DockPosition::CENTER);
    }

    void SandboxLayer::onDetach() 
    {

    }

    void SandboxLayer::onEvent() 
    {

    }

    void SandboxLayer::onUpdate() 
    {
        int32_t width, height;
        glfwGetFramebufferSize(m_Window, &width, &height);

        static float left = 0.0f, 
              right = (float)width, 
              bottom = 0.0f, 
              top = (float)height, 
              near = 0.0f, 
              far = 100.0f,
              fov = 45.0f;

        static float pos_x = 0.0f, 
                     pos_y = 0.0f, 
                     pos_z = 0.0f,
                     scale_x = 500.0f,
                     scale_y = 300.0f,
                     scale_z = 1.0f,
                     rotate_x = 0.0f,
                     rotate_y = 0.0f,
                     rotate_z = 0.0f;

        static bool show_view_projection_window = false;
        static bool show_transform_window = false;


        if (ImGui::BeginMainMenuBar())
        {
            if (ImGui::BeginMenu("Tools"))
            {
                if (ImGui::MenuItem("Projection settings"))
                {
                    show_view_projection_window ^= true;
                }
                if (ImGui::MenuItem("Object transform"))
                {
                    show_transform_window ^= true;
                }
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }

        if (show_view_projection_window)
        {
            ImGui::Begin("Projection settings", &show_view_projection_window);
            ImGui::SliderFloat("Left", &left, 0.0f, 2.0f*width);
            ImGui::SliderFloat("Right", &right, 0.0f, 2.0f*width);
            ImGui::SliderFloat("Bottom", &bottom, 0.0f, 2.0f*height);
            ImGui::SliderFloat("Top", &top, 0.0f, 2.0f*height);
            ImGui::SliderFloat("Near", &near, 0.0f, 1000.0f);
            ImGui::SliderFloat("Far", &far, 0.1f, 1000.0f);
            ImGui::SliderAngle("FOV", &fov, 0.0f, 180.0f);
            ImGui::End();
        }

        if (show_transform_window)
        {
            ImGui::Begin("Object Transform", &show_transform_window);
            ImGui::Text("Translate");
            ImGui::SliderFloat("X##pos", &pos_x, left, right);
            ImGui::SliderFloat("Y##pos", &pos_y, bottom, top);
            ImGui::SliderFloat("Z##pos", &pos_z, near, far);
            ImGui::Separator();
            ImGui::Text("Scale");
            ImGui::SliderFloat("X##scale", &scale_x, 1.0f, right);
            ImGui::SliderFloat("Y##scale", &scale_y, 1.0f, top);
            ImGui::SliderFloat("Z##scale", &scale_z, 1.0f, 50.0f);
            ImGui::Separator();
            ImGui::Text("Rotate");
            ImGui::SliderAngle("X##rot", &rotate_x, 0.0f, 360.0f);
            ImGui::SliderAngle("Y##rot", &rotate_y, 0.0f, 360.0f);
            ImGui::SliderAngle("Z##rot", &rotate_z, 0.0f, 360.0f);
            ImGui::End();
        }

        // Render::Renderer::begin(glm::perspective(glm::radians(fov), ((float)right)/((float)top), near, far));
        Render::Renderer::begin(glm::ortho(left, right, bottom, top, near, far));
        // Render::Renderer::begin(glm::mat4(1.0f));

        glm::vec4 clear_color = glm::vec4(1.0f, 1.0f, 1.0f, 1.00f);
        Render::RenderAPI::setClearColor({clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w});
        Render::RenderAPI::clear();

        shader->bind();
        shader->setFloat3("u_Color", glm::vec3{1.0f, 0.0f, 0.0f});

        pos_x = right / 2;
        pos_y = top / 2;

        glm::mat4 transform(1.0f);
        transform = glm::translate(transform, glm::vec3(pos_x, pos_y, -pos_z));
        transform = glm::scale(transform, glm::vec3(scale_x, scale_x * top / right, scale_z));
        transform = glm::rotate(transform, glm::radians(rotate_x), glm::vec3(1.0f, 0.0f, 0.0f));
        transform = glm::rotate(transform, glm::radians(rotate_y), glm::vec3(0.0f, 1.0f, 0.0f));
        transform = glm::rotate(transform, glm::radians(rotate_z), glm::vec3(0.0f, 0.0f, 1.0f));

        cv::Mat frame;
        decoder->nextFrame(frame);
        ref<Render::Texture> texture = CreateRef<Render::Texture>(frame);

        {
            ImGui::Begin("Graphic view");

            ImVec2 wsize = ImGui::GetWindowSize();
            float ratio = (float)texture->getWidth() / (float)texture->getHeight();
            float _width = 0, _height = 0;

            if ((wsize.x / ratio) > wsize.y)
            {
                _width = wsize.y * ratio;
                _height = wsize.y;
            }else {
                _width = wsize.x;
                _height = wsize.x / ratio;   
            }
            
            ImGui::SetCursorPosX((ImGui::GetWindowWidth() - _width) / 2.0f);
            ImGui::SetCursorPosY((ImGui::GetWindowHeight() - _height) / 2.0f);
            
            ImGui::Image((void*)(intptr_t)texture->getRenderID(), ImVec2(_width, _height));
            
            ImGui::End();
        }

        Render::Renderer::end();
    }
}