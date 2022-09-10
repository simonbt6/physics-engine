#pragma once

#include "../../../def.hpp"

namespace Graphics
{
    namespace Render
    {
        class UniformBuffer
        {
        protected:
            uint32_t m_RenderID;
        public:
            UniformBuffer(uint32_t size, uint32_t binding);
            virtual ~UniformBuffer();

            virtual void setData(const void* data, uint32_t size, uint32_t offset = 0);
        };
    };
};