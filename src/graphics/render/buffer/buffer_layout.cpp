#include "buffer_layout.hpp"

using namespace Graphics::Render;


BufferLayout::BufferLayout()
{

}

BufferLayout::BufferLayout(std::initializer_list<BufferElement> elements)
{
    m_Elements = elements;
    
    size_t offset = 0;
    m_Stride = 0;
    for (auto& element : m_Elements)
    {
        element.Offset = offset;
        offset += element.Size;
        m_Stride += element.Size;
    }
}

uint32_t BufferLayout::getStride() const
{
    return m_Stride;
}

const std::vector<BufferElement>& BufferLayout::getElements() const
{
    return m_Elements;
}

