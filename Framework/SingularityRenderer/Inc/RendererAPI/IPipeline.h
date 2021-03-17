#ifndef SINGULARITY_ENGINE_RENDER_RENDERER_CONTEXT
#define SINGULARITY_ENGINE_RENDER_RENDERER_CONTEXT
#include "Common.h"
#include "RendererAPI/IShader.h"
#include "RendererAPI/IRenderPass.h"
#include "RendererAPI/VertexTypes.h"
namespace SingularityEngine::SERenderer
{
	struct PipelineSpecification
	{
		std::shared_ptr<IShader> shader;
		std::shared_ptr<IRenderPass> renderPass;
		uint32_t vertexFormat;
	};

	class IPipeline
	{
	public:
		virtual ~IPipeline() = default;
		virtual PipelineSpecification& getSpecification() = 0;
		virtual const PipelineSpecification& getSpecification() const = 0;
		virtual void invalidate() = 0;
		virtual void bind() = 0;
		static std::shared_ptr<IPipeline> Create(PipelineSpecification spec);
	private:

	};


}

#endif 
