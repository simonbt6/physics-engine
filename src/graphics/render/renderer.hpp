#pragma once

#include "../../def.hpp"

#include "shader.hpp"
#include "texture.hpp"
#include "buffer/vertex_array.hpp"

namespace Graphics
{
    namespace Render
    {
        class Texture;
        class Shader;
        class Renderer
        {
        public:
            static void init();
            static void shutdown();

            static void onWindowResize(uint32_t width, uint32_t height);

            static void begin(const glm::mat4& view);
            static void end();
            static void flush();
            static void startBatch();

            static void submit(const ref<Shader>& shader, const ref<VertexArray>& vao, const glm::mat4& transform = glm::mat4(1.0f));

        public:
            static void drawQuad(const glm::vec2& position, float rotation, const glm::vec2& size, const glm::vec4& color);
            static void drawQuad(const glm::vec3& position, float rotation, const glm::vec2& size, const glm::vec4& color);
            static void drawQuad(const glm::vec2& position, float rotation, const glm::vec2& size, const ref<Texture>& texture, float tiling = 1.0f, const glm::vec4& tint = glm::vec4(1.0f));
            static void drawQuad(const glm::vec3& position, float rotation, const glm::vec2& size, const ref<Texture>& texture, float tiling = 1.0f, const glm::vec4& tint = glm::vec4(1.0f));
            static void drawQuad(const glm::mat4& transform, const glm::vec4& color);
            static void drawQuad(const glm::mat4& transform, const ref<Texture>& texture, float tiling = 1.0f, const glm::vec4& tint = glm::vec4(1.0f));

            static void drawCircle(const glm::mat4& transform, const glm::vec4& color);
            static void drawCircle(const glm::mat4& transform, const ref<Texture>& texture, float tiling = 1.0f, const glm::vec4& color = glm::vec4(1.0f));

        };
    };
};