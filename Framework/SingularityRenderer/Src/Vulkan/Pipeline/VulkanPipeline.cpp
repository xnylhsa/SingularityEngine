#include "Precompiled.h"
#include "vulkan/Pipeline/VulkanPipeline.h"
#include "RendererAPI/Renderer.h"
#include "vulkan/Device/VulkanDevice.h"
#include "vulkan/Shader/VulkanShader.h"
#include "vulkan/Util/VulkanFunctions.h"
namespace SingularityEngine::SERenderer
{
	VulkanPipeline::VulkanPipeline(const PipelineSpecification& spec) : mSpecification(spec)
	{
		UNREFERENCED_PARAMETER(spec);
	}

	VulkanPipeline::~VulkanPipeline()
	{

	}

	void VulkanPipeline::invalidate()
	{
		std::shared_ptr<VulkanDevice> device = std::dynamic_pointer_cast<VulkanDevice>(Renderer::Get()->getGraphicsDevice());
		std::shared_ptr<VulkanShader> shader = std::dynamic_pointer_cast<VulkanShader>(mSpecification.shader);
		
		VkPipelineLayoutCreateInfo pipelineCreateInfo = {};
		pipelineCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineCreateInfo.pNext = nullptr;
		pipelineCreateInfo.setLayoutCount = 1;
		//pipelineCreateInfo.pSetLayouts = shader->getDescriptorSets();
		//VkResult result = vkCreatePipelineLayout(device->getLogicalDevice(), &pipelineCreateInfo, nullptr, mPipelineLayout);
		//ASSERT(result == VK_SUCCESS, "[SERenderer::VulkanPipeline] Failed to create PipelineLayout");


	}

	void VulkanPipeline::bind()
	{

	}
}

