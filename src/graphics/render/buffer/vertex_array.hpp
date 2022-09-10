#pragma once

#include "../../../def.hpp"

#include "vertex_buffer.hpp"
#include "index_buffer.hpp"

namespace Graphics
{
    namespace Render
    {
        class VertexArray
        {
        protected:
            uint32_t m_RenderID;
            uint32_t m_VBO_Index = 0;
            std::vector<ref<VertexBuffer>> m_VBO;
            ref<IndexBuffer> m_IBO;

        public:
            VertexArray();
            virtual ~VertexArray();

            virtual void bind() const;
            virtual void unbind() const;

            virtual void addVertexBuffer(const ref<VertexBuffer>& vbo);
            virtual void setIndexBuffer(const ref<IndexBuffer>& ibo);

            inline virtual const std::vector<ref<VertexBuffer>>& getVertexBuffers() const { return m_VBO; }
            inline virtual const ref<IndexBuffer>& getIndexBuffer() const { return m_IBO; }
        };
    };
};