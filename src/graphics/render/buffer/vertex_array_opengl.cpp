#include "vertex_array.hpp"

#include <glad/glad.h>

using namespace Graphics::Render;

VertexArray::VertexArray()
{
    glCreateVertexArrays(1, &m_RenderID);
}

VertexArray::~VertexArray()
{
    glDeleteVertexArrays(1, &m_RenderID);
}

void VertexArray::bind() const
{
    glBindVertexArray(m_RenderID);
}

void VertexArray::unbind() const
{
    glBindVertexArray(0);
}

void VertexArray::addVertexBuffer(const ref<VertexBuffer>& vbo)
{
    if (!vbo->getLayout().getElements().size())
    {
        std::cerr << "Vertex Buffer has no layout." << std::endl;
        return;
    }

    this->bind();
    vbo->bind();

    const auto& layout = vbo->getLayout();
    for (const auto& element : layout)
    {
        switch (element.Type)
        {
            case ShaderDatatype::Float:
            case ShaderDatatype::Float2:
            case ShaderDatatype::Float3:
            case ShaderDatatype::Float4:
            {
                glEnableVertexAttribArray(m_VBO_Index);
                glVertexAttribPointer(
                    m_VBO_Index,
                    element.getComponentCount(),
                    GL_FLOAT,
                    element.Normalized ? GL_TRUE : GL_FALSE,
                    layout.getStride(),
                    (const void*) element.Offset);
                m_VBO_Index++;
                break;
            };

            case ShaderDatatype::Int:
            case ShaderDatatype::Int2:
            case ShaderDatatype::Int3:
            case ShaderDatatype::Int4:
            case ShaderDatatype::Bool:
            {
                glEnableVertexAttribArray(m_VBO_Index);
                glVertexAttribIPointer(
                    m_VBO_Index,
                    element.getComponentCount(),
                    element.Type == ShaderDatatype::Bool ? GL_BOOL : GL_INT,
                    layout.getStride(),
                    (const void*) element.Offset);
                m_VBO_Index++;
                break;
            };

            case ShaderDatatype::Mat3:
            case ShaderDatatype::Mat4:
            {
                uint8_t count = element.getComponentCount();
                for (uint8_t i = 0; i < count; i++)
                {
                    glEnableVertexAttribArray(m_VBO_Index);
                    glVertexAttribPointer(
                        m_VBO_Index,
                        element.getComponentCount(),
                        GL_FLOAT,
                        element.Normalized ? GL_TRUE : GL_FALSE,
                        layout.getStride(),
                        (const void*)(element.Offset + sizeof(float) * count * i));
                    m_VBO_Index++;
                }
                break;
            };
        };
    }

    m_VBO.push_back(vbo);
}

void VertexArray::setIndexBuffer(const ref<IndexBuffer>& ibo)
{
    this->bind();
    ibo->bind();

    m_IBO = ibo;
}