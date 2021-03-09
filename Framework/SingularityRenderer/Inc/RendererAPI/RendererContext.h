#ifndef SINGULARITY_ENGINE_RENDER_RENDERER_CONTEXT
#define SINGULARITY_ENGINE_RENDER_RENDERER_CONTEXT
#include "Common.h"

namespace SingularityEngine::SERenderer
{
	class IRenderContext
	{
	public:
		virtual ~IRenderContext() = default;
		virtual bool initialize() = 0;
		virtual bool teardown() = 0;
		static std::unique_ptr<IRenderContext> Create();
	};
}

#endif 
