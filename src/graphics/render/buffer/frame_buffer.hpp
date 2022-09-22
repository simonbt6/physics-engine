#pragma once

#include "../../../def.hpp"
#include <initializer_list>

namespace Graphics
{
    enum class FramebufferTextureFormat
    {
        NONE = 0,
        RGBA8,
        RED_INTEGER,
        DEPTH24_STENCIL8,
        DEPTH = DEPTH24_STENCIL8
    };

    struct FramebufferTextureSpecification
    {
        FramebufferTextureSpecification() = default;
        FramebufferTextureSpecification(FramebufferTextureFormat fmt)
            :TextureFormat(fmt){}

        FramebufferTextureFormat TextureFormat = FramebufferTextureFormat::NONE;
    };

    struct FramebufferAttachmentSpecification
    {
        FramebufferAttachmentSpecification() = default;
        FramebufferAttachmentSpecification(std::initializer_list<FramebufferTextureSpecification> attachments)
            :Attachments(attachments){}

        std::vector<FramebufferTextureSpecification> Attachments;
    };

    struct FramebufferSpecification
    {
        uint32_t Width = 0;
        uint32_t Height = 0;
        FramebufferAttachmentSpecification Attachments;
        uint32_t samples = 1;

        bool SwapchainTarget = false;
    };

    class Framebuffer
    {
    protected:
        uint32_t m_RenderID = 0;
        FramebufferSpecification m_Spec;

        std::vector<FramebufferTextureSpecification> m_ColorAttachmentSpec;
        FramebufferTextureSpecification m_DepthAttachmentSpec = FramebufferTextureFormat::NONE;

        std::vector<uint32_t> m_ColorAttachments;
        uint32_t m_DepthAttachment;

    public:
        Framebuffer(const FramebufferSpecification& spec);
        virtual ~Framebuffer();

        void invalidate();

        virtual void bind() = 0;
        virtual void unbind() = 0;

        virtual void resize(uint32_t width, uint32_t height) = 0;
        virtual int readPixel(uint32_t index, int value) = 0;

        virtual void clearAttachment(uint32_t index, int value) = 0;

        virtual uint32_t getColorAttachmentRenderID(uint32_t index = 0) const = 0;    

        virtual const FramebufferSpecification& getSpecification() const = 0;
    };
};