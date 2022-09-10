#pragma once

#include "../../def.hpp"

#include <glad/glad.h>

namespace Graphics
{
    namespace Render
    {
        class Texture
        {
        protected:
            std::string m_Path;
            bool m_IsLoaded = false;
            uint32_t m_Width, m_Height;
            uint32_t m_RenderID;
            GLenum m_InternalFormat, m_DataFormat;

        public:
            Texture(uint32_t width, uint32_t height);
            Texture(const std::string& path);

            virtual ~Texture();
            
            virtual uint32_t getWidth() const;
            virtual uint32_t getHeight() const;
            virtual uint32_t getRenderID() const;

            virtual const std::string& getPath() const;
            virtual void setData(void* data, uint32_t size) const;
            virtual void bind(uint32_t slot = 0) const;
            virtual bool isLoaded() const;

            virtual bool operator==(const Texture& other) const;
        };
    };
};