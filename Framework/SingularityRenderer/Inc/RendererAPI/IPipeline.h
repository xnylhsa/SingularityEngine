#ifndef SINGULARITY_ENGINE_RENDER_RENDERER_CONTEXT
#define SINGULARITY_ENGINE_RENDER_RENDERER_CONTEXT
#include "Common.h"
#include "RendererAPI/IShader.h"
#include "RendererAPI/Buffer.h"
namespace SingularityEngine::SERenderer
{
	struct PipelineSpecification
	{
		std::shared_ptr<IShader> shader;
		//VertexBufferLayout layout;
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
