#include "uniform_buffer.hpp"

#include <glad/glad.h>

using namespace Graphics::Render;

UniformBuffer::UniformBuffer(uint32_t size, uint32_t binding)
{
    glCreateBuffers(1, &m_RenderID);
    glNamedBufferData(m_RenderID, size, nullptr, GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, binding, m_RenderID);
}

UniformBuffer::~UniformBuffer()
{
    glDeleteBuffers(1, &m_RenderID);
}

void UniformBuffer::setData(const void* data, uint32_t size, uint32_t offset)
{
    glNamedBufferSubData(m_RenderID, offset, size, data);
}