#include "Precompiled.h"
#include "RendererAPI/IPipeline.h"
#include "vulkan/Pipeline/VulkanPipeline.h"
#include "RendererAPI/Renderer.h"

namespace SingularityEngine::SERenderer
{
	std::shared_ptr<SingularityEngine::SERenderer::IPipeline> IPipeline::Create(PipelineSpecification spec)
	{
		switch (Renderer::GetAPI())
		{
		case RenderingAPI::None:    ASSERT(false, "[Renderer::Shader] None is currently not supported!"); return nullptr;
		case RenderingAPI::Vulkan:  return std::make_shared<VulkanPipeline>(spec);
		}
		//load into shader library
		ASSERT(false, "[Renderer::Shader] Unknown RendererAPI!");
		return nullptr;
	}
}


