#ifndef SINGULARITY_ENGINE_RENDERER_RENDER_PASS
#define SINGULARITY_ENGINE_RENDERER_RENDER_PASS
#include "Common.h"
#include "RendererAPI/IFramebuffer.h"

namespace SingularityEngine::SERenderer
{

	struct RenderPassSpecification
	{
		std::shared_ptr<IFramebuffer> targetFrameBuffer;
	};

	class IRenderPass
	{
	public:
		virtual ~IRenderPass() = default;
		virtual RenderPassSpecification& getSpecification() = 0;
		virtual const RenderPassSpecification& getSpecification() const = 0;

		static std::shared_ptr<IRenderPass> Create(const RenderPassSpecification& spec);
	};
}

#endif 
