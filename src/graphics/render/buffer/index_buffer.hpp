#pragma once

#include "../../../def.hpp"

namespace Graphics
{
    namespace Render
    {
        class IndexBuffer
        {
        protected:
            uint32_t m_RenderID;
            uint32_t m_Count;

        public:
            IndexBuffer(uint32_t* indices, uint32_t count);
            virtual ~IndexBuffer();

            virtual void bind() const;
            virtual void unbind() const;

            virtual uint32_t getCount() const { return m_Count; }
        };
    };
};