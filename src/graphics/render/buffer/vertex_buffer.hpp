#pragma once

#include "../../../def.hpp"
#include "buffer_layout.hpp"

namespace Graphics
{
    namespace Render
    {
        class VertexBuffer
        {
        protected:
            uint32_t m_RenderID;
            BufferLayout m_Layout;
            
        public:
            VertexBuffer(uint32_t size);
            VertexBuffer(float* vertices, uint32_t size);

            virtual ~VertexBuffer();

            virtual void bind() const;
            virtual void unbind() const;

            virtual void setData(const void* data, uint32_t size);

            inline virtual const BufferLayout& getLayout() const { return m_Layout; }
            inline virtual void setLayout(const BufferLayout& layout) { m_Layout = layout; }
        };
    };
};