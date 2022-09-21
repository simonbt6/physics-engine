#include "texture.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

using namespace Graphics::Render;

Texture::Texture(cv::Mat& frame)
{
    m_Width = frame.cols;
    m_Height = frame.rows;
    m_InternalFormat = GL_RGB;
    m_DataFormat = frame.channels() == 1 ? GL_LUMINANCE : GL_BGR;

    // cv::flip(frame, frame, 0);

    glGenTextures(1, &m_RenderID);
    glBindTexture(GL_TEXTURE_2D, m_RenderID);

    glTextureParameteri(m_RenderID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(m_RenderID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTextureParameteri(m_RenderID, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(m_RenderID, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexImage2D(GL_TEXTURE_2D, 0, m_InternalFormat, m_Width, m_Height, 0, m_DataFormat, GL_UNSIGNED_BYTE, frame.ptr());

}

Texture::Texture(uint32_t width, uint32_t height)
    :m_Width(width), m_Height(height)
{
    m_InternalFormat = GL_RGBA8;
    m_DataFormat = GL_RGBA;

    glCreateTextures(GL_TEXTURE_2D, 1, &m_RenderID);
    glTextureStorage2D(m_RenderID, 1, m_InternalFormat, m_Width, m_Height);

    glTextureParameteri(m_RenderID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(m_RenderID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTextureParameteri(m_RenderID, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(m_RenderID, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

Texture::Texture(const std::string& path)
{
    int width, height, channels;
    stbi_set_flip_vertically_on_load(1);
    stbi_uc* data = nullptr;
    {
        data = stbi_load(path.c_str(), &width, &height, &channels, 0);
    }

    if (data)
    {
        m_IsLoaded = true;

        m_Width = width;
        m_Height = height;

        if (channels == 4)
        {
            m_InternalFormat = GL_RGBA8;
            m_DataFormat = GL_RGBA;
        }

        if (channels == 3)
        {
            m_InternalFormat = GL_RGBA8;
            m_DataFormat = GL_RGB;
        }

        glCreateTextures(GL_TEXTURE_2D, 1, &m_RenderID);
        glTextureStorage2D(m_RenderID, 1, m_InternalFormat, m_Width, m_Height);

        glTextureParameteri(m_RenderID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(m_RenderID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTextureParameteri(m_RenderID, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTextureParameteri(m_RenderID, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glTextureSubImage2D(m_RenderID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);

        stbi_image_free(data);
    }
}

Texture::~Texture()
{
    // glDeleteTextures(1, &m_RenderID);
}

uint32_t Texture::getWidth() const
{
    return m_Width;
}

uint32_t Texture::getHeight() const
{
    return m_Height;
}

uint32_t Texture::getRenderID() const
{
    return m_RenderID;
}

const std::string& Texture::getPath() const
{
    return m_Path;
}

void Texture::setData(void* data, uint32_t size) const
{
    uint32_t bpp = m_DataFormat == GL_RGBA ? 4 : 3;
    if (!(size == m_Width * m_Height * bpp))
    {
        printf("\nData must be entire texture.");
        return;
    }
    glTextureSubImage2D(m_RenderID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);
}

void Texture::bind(uint32_t slot) const
{
    glBindTextureUnit(slot, m_RenderID);
}

bool Texture::isLoaded() const
{
    return m_IsLoaded;
}

bool Texture::operator==(const Texture& other) const
{
    return m_RenderID == other.getRenderID();
}