#include "index_buffer.hpp"

#include <glad/glad.h>

using namespace Graphics::Render;

IndexBuffer::IndexBuffer(uint32_t* indices, uint32_t count)
:m_Count(count)
{
    glCreateBuffers(1, &m_RenderID);
    glBindBuffer(GL_ARRAY_BUFFER, m_RenderID);
    glBufferData(GL_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
}

IndexBuffer::~IndexBuffer()
{
    glDeleteBuffers(1, &m_RenderID);
}

void IndexBuffer::bind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RenderID);
}

void IndexBuffer::unbind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}