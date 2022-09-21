#include "frame_buffer.hpp"

#include <glad/glad.h>

using namespace Graphics;

GLenum texture_target(bool multisampled)
{
    return multisampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
}

void create_textures(bool multisampled, uint32_t* out, uint32_t count)
{
    glCreateTextures(texture_target(multisampled), count, out);
}

void bind_texture(bool multisampled, uint32_t id)
{
    glBindTexture(texture_target(multisampled), id);
}

void attach_color_texture(uint32_t id, int samples, GLenum internal_fmt, GLenum fmt, uint32_t width, uint32_t height, int index)
{
    bool multisampled = samples >1;
    if (multisampled)
    {
        glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, internal_fmt, width, height, GL_FALSE);
    }
    else
    {
        glTexImage2D(GL_TEXTURE_2D, 0, internal_fmt, width, height, 0, fmt, GL_UNSIGNED_BYTE, nullptr);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, texture_target(multisampled), id, 0);
}

void attach_depth_texture(uint32_t id, int samples, GLenum fmt, GLenum attachment_type, uint32_t width, uint32_t height)
{
    bool multisampled = samples >1;
    if (multisampled)
    {
        glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, fmt, width, height, GL_FALSE);
    }
    else
    {
        glTexStorage2D(GL_TEXTURE_2D, 1, fmt, width, height);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }

    glFramebufferTexture2D(GL_FRAMEBUFFER, attachment_type, texture_target(multisampled), id, 0);
}

bool is_depth_format(FramebufferTextureFormat fmt)
{
    switch(fmt)
    {
        case FramebufferTextureFormat::DEPTH24_STENCIL8: 
            return true;
        default: 
            return false;
    }
}

GLenum framebuffer_texture_format_to_gl(FramebufferTextureFormat fmt)
{
    switch(fmt)
    {
        case FramebufferTextureFormat::RGBA8:
            return GL_RGBA8;
        case FramebufferTextureFormat::RED_INTEGER:
            return GL_RED_INTEGER;
        default:
            return 0;
    };
}

Framebuffer::Framebuffer(const FramebufferSpecification& spec)
    :m_Spec(spec)
{
    for (auto spec : m_Spec.Attachments.Attachments)
    {
        if (!is_depth_format(spec.TextureFormat))
            m_ColorAttachmentSpec.emplace_back(spec);
        else
            m_DepthAttachmentSpec = spec;
    }
    invalidate();
}

Framebuffer::~Framebuffer()
{
    glDeleteFramebuffers(1, &m_RenderID);
    glDeleteTextures(m_ColorAttachments.size(), m_ColorAttachments.data());
    glDeleteTextures(1, &m_DepthAttachment);
}

void Framebuffer::invalidate()
{
    if (m_RenderID)
    {
        glDeleteFramebuffers(1, &m_RenderID);
        glDeleteTextures(m_ColorAttachments.size(), m_ColorAttachments.data());
        glDeleteTextures(1, &m_DepthAttachment);

        m_ColorAttachments.clear();
        m_DepthAttachment = 0;
    }

    glCreateFramebuffers(1, &m_RenderID);
    glBindFramebuffer(GL_FRAMEBUFFER, m_RenderID);

    bool multisample = m_Spec.samples > 1;

    if (m_ColorAttachmentSpec.size())
    {
        m_ColorAttachments.resize(m_ColorAttachmentSpec.size());
        create_textures(multisample, m_ColorAttachments.data(), m_ColorAttachments.size());

        for (size_t i = 0; i < m_ColorAttachments.size(); i++)
        {
            bind_texture(multisample, m_ColorAttachments[i]);
            switch (m_ColorAttachmentSpec[i].TextureFormat)
            {
                case FramebufferTextureFormat::RGBA8:
                    attach_color_texture(m_ColorAttachments[i], m_Spec.samples, GL_RGBA8, GL_RGBA, m_Spec.Width, m_Spec.Height, i);
                    break;
                case FramebufferTextureFormat::RED_INTEGER:
                    attach_color_texture(m_ColorAttachments[i], m_Spec.samples, GL_R32I, GL_RED_INTEGER, m_Spec.Width, m_Spec.Height, i);
                    break;
            }
        }
    }

}