#include "vertex_buffer.hpp"

#include <glad/glad.h>

using namespace Graphics::Render;

VertexBuffer::VertexBuffer(uint32_t size)
{
    glCreateBuffers(1, &m_RenderID);
    glBindBuffer(GL_ARRAY_BUFFER, m_RenderID);
    glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
}

VertexBuffer::VertexBuffer(float* vertices, uint32_t size)
{
    glCreateBuffers(1, &m_RenderID);
    glBindBuffer(GL_ARRAY_BUFFER, m_RenderID);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_DYNAMIC_DRAW);
}

VertexBuffer::~VertexBuffer()
{
    glDeleteBuffers(1, &m_RenderID);
}

void VertexBuffer::bind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, m_RenderID);
}

void VertexBuffer::unbind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexBuffer::setData(const void* data, uint32_t size)
{
    this->bind();
    glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
}