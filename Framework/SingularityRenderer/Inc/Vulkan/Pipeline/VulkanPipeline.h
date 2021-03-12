#ifndef SINGULARITY_ENGINE_VULKAN_PIPELINE
#define SINGULARITY_ENGINE_VULKAN_PIPELINE

#include "Common.h"
#include "RendererAPI/IPipeline.h"

namespace SingularityEngine::SERenderer
{
	class VulkanPipeline : public IPipeline
	{
	public:
		VulkanPipeline(const PipelineSpecification& spec);
		~VulkanPipeline();
		PipelineSpecification& getSpecification() { return mSpecification; }
		const PipelineSpecification& getSpecification() const { return mSpecification; }

		void invalidate() override;
		void bind() override;

		VkPipeline getVulkanPipeline() { return mPipeline; }
	private:
		PipelineSpecification mSpecification;
		VkPipeline mPipeline = nullptr;
		VkPipelineLayout mPipelineLayout = nullptr;
		VkPipelineCache mPipelineCache = nullptr;
	};
}



#endif
