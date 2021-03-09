#ifndef SINGULARITY_ENGINE_RENDER_RENDERER_CONTEXT
#define SINGULARITY_ENGINE_RENDER_RENDERER_CONTEXT
#include "Common.h"

namespace SingularityEngine::SERenderer
{



	struct PipelineSpecification
	{

	};

	class IPipeline
	{
	public:
		virtual ~IPipeline();
		virtual PipelineSpecification& getSpecification() = 0;
		virtual const PipelineSpecification& getSpecification() const = 0;

		virtual void invalidate() = 0;
		virtual void bind() = 0;
	private:

	};

}

#endif 
