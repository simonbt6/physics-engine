#pragma once

#include "../../../def.hpp"

namespace Graphics
{
    namespace Render
    {
        enum class ShaderDatatype
        {
            None,
            Float,
            Float2,
            Float3,
            Float4,
            Mat3,
            Mat4,
            Int,
            Int2,
            Int3,
            Int4,
            Bool
        };

        static uint32_t getShaderDatatypeSize(ShaderDatatype type)
        {
            switch (type)
            {
                case ShaderDatatype::Float:  return 4;
                case ShaderDatatype::Float2: return 4 * 2;
                case ShaderDatatype::Float3: return 4 * 3;
                case ShaderDatatype::Float4: return 4 * 4;

                case ShaderDatatype::Mat3:   return 4 * 3 * 3;
                case ShaderDatatype::Mat4:   return 4 * 4 * 4;

                case ShaderDatatype::Int:    return 4;
                case ShaderDatatype::Int2:   return 4 * 2;
                case ShaderDatatype::Int3:   return 4 * 3;
                case ShaderDatatype::Int4:   return 4 * 4;
                case ShaderDatatype::Bool:   return 1;
                default:                     return 0;
            };
        }

        struct BufferElement
        {
            std::string Name;
            ShaderDatatype Type;
            uint32_t Size;
            size_t Offset;
            bool Normalized;

            BufferElement() = default;
            BufferElement(ShaderDatatype type, const std::string& name, bool normalized = false)
            :Name(name), Type(type), Size(getShaderDatatypeSize(type)), Offset(0), Normalized(normalized) {}

            uint32_t getComponentCount() const
            {
                switch (Type)
                {
                    case ShaderDatatype::Float:     return 1;
                    case ShaderDatatype::Float2:    return 2;
                    case ShaderDatatype::Float3:    return 3;
                    case ShaderDatatype::Float4:    return 4;
                    case ShaderDatatype::Mat3:      return 3;
                    case ShaderDatatype::Mat4:      return 4;
                    case ShaderDatatype::Int:       return 1;
                    case ShaderDatatype::Int2:      return 2;
                    case ShaderDatatype::Int3:      return 3;
                    case ShaderDatatype::Int4:      return 4;
                    case ShaderDatatype::Bool:      return 1;
                    default:                        return 0;
                };
            }
        };

        class BufferLayout
        {
        private:
            std::vector<BufferElement> m_Elements;
            uint32_t m_Stride = 0;

        public:
            BufferLayout();
            BufferLayout(std::initializer_list<BufferElement> elements);

            uint32_t getStride() const;
            const std::vector<BufferElement>& getElements() const;

            inline std::vector<BufferElement>::iterator begin()             { return m_Elements.begin(); }
            inline std::vector<BufferElement>::iterator end()               { return m_Elements.end();   }
            inline std::vector<BufferElement>::const_iterator begin() const { return m_Elements.begin(); }
            inline std::vector<BufferElement>::const_iterator end()   const { return m_Elements.end();   }
           
        };
    };
};