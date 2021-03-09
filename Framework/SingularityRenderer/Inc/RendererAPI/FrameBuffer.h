#ifndef SINGULARITY_ENGINE_RENDER_RENDERER_CONTEXT
#define SINGULARITY_ENGINE_RENDER_RENDERER_CONTEXT
#include "Common.h"

namespace SingularityEngine::SERenderer
{
	enum class FramebufferTextureFormat
	{
		None = 0,

		// Color
		RGBA8,
		SRGB,
		// Depth/stencil
		DEPTH32FLOATSTENCIL8UINT,
		DEPTH24UNORMSTENCIL8UINT,
		// Defaults
		Depth = DEPTH32FLOATSTENCIL8UINT,
		Color = SRGB
	};

	struct FramebufferTextureSpecification
	{
		FramebufferTextureSpecification() = default;
		FramebufferTextureSpecification(FramebufferTextureFormat format)
			: TextureFormat(format) {}

		FramebufferTextureFormat TextureFormat = FramebufferTextureFormat::None;
	};

	struct FramebufferAttachmentSpecification
	{
		FramebufferAttachmentSpecification() = default;
		FramebufferAttachmentSpecification(std::initializer_list<FramebufferTextureSpecification> attachments)
			: Attachments(attachments) {}

		std::vector<FramebufferTextureSpecification> Attachments;
	};

	struct FramebufferSpecification
	{
		uint32_t Width = 0, Height = 0;
		FramebufferAttachmentSpecification Attachments;
		uint32_t Samples = 1;

		bool SwapChainTarget = false;
	};

	class Framebuffer
	{
	public:
		virtual ~Framebuffer() = default;

		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		virtual void Resize(uint32_t width, uint32_t height) = 0;

		virtual uint32_t GetColorAttachmentRendererID(uint32_t index = 0) const = 0;

		virtual const FramebufferSpecification& GetSpecification() const = 0;

		static std::shared_ptr <Framebuffer> Create(const FramebufferSpecification& spec);
	};

}

#endif 