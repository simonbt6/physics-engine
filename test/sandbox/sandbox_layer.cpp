#include "sandbox_layer.hpp"

#include <imgui.h>

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

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


    void SandboxLayer::onAttach(GLFWwindow* window) 
    {
        m_Window = window;

        shader = CreateRef<Render::Shader>("test", "D:/dev-repo/PhysicsEngine/test/sandbox/shaders/");

        vao = CreateRef<Render::VertexArray>();
        vbo = CreateRef<Render::VertexBuffer>(vertices, sizeof(vertices));
        ibo = CreateRef<Render::IndexBuffer>(indices, sizeof(indices) / sizeof(uint32_t));

        vbo->setLayout({
            { Render::ShaderDatatype::Float3, "a_Position" },
            { Render::ShaderDatatype::Float3, "a_Color"}
        });
        vao->addVertexBuffer(vbo);
        vao->setIndexBuffer(ibo);

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
                     scale_x = 1.0f,
                     scale_y = 1.0f,
                     scale_z = 1.0f,
                     rotate_x = 0.0f,
                     rotate_y = 0.0f,
                     rotate_z = 0.0f;

        static bool show_demo_window = true;
        static bool show_view_projection_window = true;
        static bool show_transform_window = true;

        // if (show_demo_window)
        //     ImGui::ShowDemoWindow(&show_demo_window);

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
            ImGui::Begin("Object transform", &show_view_projection_window);
            ImGui::Text("Translate");
            ImGui::SliderFloat("X##pos", &pos_x, left, right);
            ImGui::SliderFloat("Y##pos", &pos_y, bottom, top);
            ImGui::SliderFloat("Z##pos", &pos_z, near, far);
            ImGui::Separator();
            ImGui::Text("Scale");
            ImGui::SliderFloat("X##scale", &scale_x, 1.0f, 50.0f);
            ImGui::SliderFloat("Y##scale", &scale_y, 1.0f, 50.0f);
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

        glm::vec4 clear_color = glm::vec4(0.65f, 0.45f, 0.55f, 1.00f);
        Render::RenderAPI::setClearColor({clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w});
        Render::RenderAPI::clear();

        shader->bind();
        shader->setFloat3("u_Color", glm::vec3{1.0f, 0.0f, 0.0f});

        glm::mat4 transform(1.0f);
        transform = glm::translate(transform, glm::vec3(pos_x, pos_y, -pos_z));
        transform = glm::scale(transform, glm::vec3(scale_x, scale_y, scale_z));
        transform = glm::rotate(transform, glm::radians(rotate_x), glm::vec3(1.0f, 0.0f, 0.0f));
        transform = glm::rotate(transform, glm::radians(rotate_y), glm::vec3(0.0f, 1.0f, 0.0f));
        transform = glm::rotate(transform, glm::radians(rotate_z), glm::vec3(0.0f, 0.0f, 1.0f));

        Render::Renderer::drawQuad(glm::vec3(100.0f, 100.0f, -5.0f), 0.0f, glm::vec2(100.0f, 200.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));

        Render::Renderer::submit(shader, vao, transform);

        Render::Renderer::end();
    }
}