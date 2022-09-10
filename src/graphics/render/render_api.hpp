#pragma once

#include "../../def.hpp"

#include "buffer/vertex_array.hpp"

namespace Graphics
{
    namespace Render
    {
        class RenderAPI
        {
        public:
            static void init();
            static void clear();

            static void setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height);
            static void setClearColor(const glm::vec4& color);
            static void setLineWidth(float width);

            static void drawIndexed(const ref<VertexArray>& vao, uint32_t index_count = 0);
            static void drawLines(const ref<VertexArray>& vao, uint32_t vertex_count);

        };
    };
};