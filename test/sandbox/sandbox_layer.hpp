#pragma once

#include "../../src/def.hpp"
#include "../../src/graphics/Graphics.hpp"

using namespace Graphics;

namespace Test
{
    class SandboxLayer : public Layer
    {
        GLFWwindow* m_Window;
        ref<Render::VertexArray>     vao;
        ref<Render::VertexBuffer>    vbo;
        ref<Render::IndexBuffer>     ibo;
        ref<Render::Shader> shader;

        virtual void onAttach(GLFWwindow*) override;
        virtual void onDetach() override;
        virtual void onEvent() override;
        virtual void onUpdate() override;
    };
};