#ifndef SINGULARITY_ENGINE_RENDER_FRAME_BUFFER
#define SINGULARITY_ENGINE_RENDER_FRAME_BUFFER
#include "Common.h"

namespace SingularityEngine::SERenderer
{
	enum class FramebufferFormat
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

	struct FramebufferSpecification
	{
		float Scale = 1.0f;
		uint32_t Width = 0;
		uint32_t Height = 0;
		Math::Color ClearColor = { 0.0f, 0.0f, 0.0f, 1.0f };
		FramebufferFormat Format = FramebufferFormat::Color;
		uint32_t Samples = 1;
		bool SwapChainTarget = false;
		std::string debugName;
	};

	class IFramebuffer
	{
	public:
		virtual ~IFramebuffer() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void Resize(uint32_t width, uint32_t height) = 0;
		virtual void AddResizeCallback(const std::function<void(std::shared_ptr<IFramebuffer>)>& func) = 0;

		virtual void BindTexture(uint32_t slot) const = 0;

		virtual uint32_t getWidth() const = 0;
		virtual uint32_t getHeight() const = 0;

		virtual const FramebufferSpecification& GetSpecification() const = 0;

		static std::shared_ptr <IFramebuffer> Create(const FramebufferSpecification& spec);
	};

}

#endif 
