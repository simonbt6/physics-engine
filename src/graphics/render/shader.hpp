#pragma once

#include "../../def.hpp"

#include <glad/glad.h>

namespace Graphics
{
    namespace Render
    {
        class Shader
        {
        protected:
            std::string m_Name;
            uint32_t m_Program;

        public:
            Shader(const std::string& name, const std::string& path);
            Shader(const std::string& name, const std::string& vertex_src, const std::string& frag_src);
            virtual ~Shader();

            virtual void bind() const;
            virtual void unbind() const;

            virtual void setInt(const std::string& name, int32_t value);
            virtual void setIntArray(const std::string& name, int32_t* values, uint32_t count);
            virtual void setFloat(const std::string& name, float value);
            virtual void setFloat2(const std::string& name, const glm::vec2& value);
            virtual void setFloat3(const std::string& name, const glm::vec3& value);
            virtual void setFloat4(const std::string& name, const glm::vec4& value);
            virtual void setMat4(const std::string& name, const glm::mat4& value);

            virtual const std::string& getName() const;
        };
    };
};