#include "Precompiled.h"
#include "vulkan/Pipeline/VulkanPipeline.h"
#include "RendererAPI/Renderer.h"
#include "vulkan/Device/VulkanDevice.h"
#include "vulkan/Shader/VulkanShader.h"
#include "vulkan/Util/VulkanFunctions.h"
#include "RendererAPI/VertexTypes.h"
#include "vulkan/SwapChain/VulkanSwapChain.h"
#include "vulkan/RenderPass/VulkanRenderPass.h"

namespace SingularityEngine::SERenderer
{
	VulkanPipeline::VulkanPipeline(const PipelineSpecification& spec) : mSpecification(spec)
	{
		invalidate();
	}

	VulkanPipeline::~VulkanPipeline()
	{
		std::shared_ptr<VulkanDevice> device = std::dynamic_pointer_cast<VulkanDevice>(Renderer::Get()->getGraphicsDevice());
		mSpecification.shader.reset();
		vkDestroyPipeline(device->getLogicalDevice(), mPipeline, nullptr);
		vkDestroyPipelineCache(device->getLogicalDevice(), mPipelineCache, nullptr);
		vkDestroyPipelineLayout(device->getLogicalDevice(), mPipelineLayout, nullptr);
	}

	void VulkanPipeline::invalidate()
	{
		std::shared_ptr<VulkanDevice> device = std::dynamic_pointer_cast<VulkanDevice>(Renderer::Get()->getGraphicsDevice());
		std::shared_ptr<VulkanShader> shader = std::dynamic_pointer_cast<VulkanShader>(mSpecification.shader);
		//get renderpass from frame buffer;
		auto* swapchain = dynamic_cast<VulkanSwapChain*>(Renderer::Get()->getSwapchain());


		auto descriptorSetLayouts = shader->getAllDescriptorSetLayouts();
		auto pushConstantRanges = shader->getPushConstantRanges();
		std::array<VkDescriptorSetLayout, 4> compactedLayouts;
		int s = 0;
		for (int i = 0; i < 4; i++) {
			if (descriptorSetLayouts[i] != VK_NULL_HANDLE) {
				compactedLayouts[s] = descriptorSetLayouts[i];
				s++;
			}
		}

		VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo = {};
		pipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutCreateInfo.pNext = nullptr;
		pipelineLayoutCreateInfo.setLayoutCount = s;
		pipelineLayoutCreateInfo.pSetLayouts = compactedLayouts.data();
		pipelineLayoutCreateInfo.pushConstantRangeCount = (uint32_t)pushConstantRanges.size();
		pipelineLayoutCreateInfo.pPushConstantRanges = pushConstantRanges.data();


		VkResult result = vkCreatePipelineLayout(device->getLogicalDevice(), &pipelineLayoutCreateInfo, nullptr, &mPipelineLayout);
		ASSERT(result == VK_SUCCESS, "[SERenderer::VulkanPipeline] Failed to create PipelineLayout");

		VkGraphicsPipelineCreateInfo pipelineCreateInfo = {};
		pipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		pipelineCreateInfo.layout = mPipelineLayout;
		pipelineCreateInfo.renderPass = (swapchain->getRenderpass()->getRenderPass());

		VkPipelineInputAssemblyStateCreateInfo inputAssemblyState = {};
		inputAssemblyState.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		inputAssemblyState.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;

		VkPipelineRasterizationStateCreateInfo rasterizationState = {};
		rasterizationState.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		rasterizationState.polygonMode = VK_POLYGON_MODE_FILL;
		rasterizationState.cullMode = VK_CULL_MODE_BACK_BIT;
		rasterizationState.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
		rasterizationState.depthClampEnable = VK_FALSE;
		rasterizationState.rasterizerDiscardEnable = VK_FALSE;
		rasterizationState.depthBiasEnable = VK_FALSE;
		rasterizationState.lineWidth = 1.0f;

		VkPipelineColorBlendAttachmentState blendAttachmentState[1] = {};
		blendAttachmentState[0].colorWriteMask = 0xf;
		blendAttachmentState[0].blendEnable = VK_FALSE;
		VkPipelineColorBlendStateCreateInfo colorBlendState = {};
		colorBlendState.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		colorBlendState.attachmentCount = 1;
		colorBlendState.pAttachments = blendAttachmentState;

		VkPipelineViewportStateCreateInfo viewportState = {};
		viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		viewportState.viewportCount = 1;
		viewportState.scissorCount = 1;

		std::vector<VkDynamicState> dynamicStateEnables;
		dynamicStateEnables.push_back(VK_DYNAMIC_STATE_VIEWPORT);
		dynamicStateEnables.push_back(VK_DYNAMIC_STATE_SCISSOR);
		VkPipelineDynamicStateCreateInfo dynamicState = {};
		dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
		dynamicState.pDynamicStates = dynamicStateEnables.data();
		dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStateEnables.size());

		VkPipelineDepthStencilStateCreateInfo depthStencilState = {};
		depthStencilState.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
		depthStencilState.depthTestEnable = VK_TRUE;
		depthStencilState.depthWriteEnable = VK_TRUE;
		depthStencilState.depthCompareOp = VK_COMPARE_OP_LESS_OR_EQUAL;
		depthStencilState.depthBoundsTestEnable = VK_FALSE;
		//depthStencilState.back.failOp = VK_STENCIL_OP_KEEP;
		//depthStencilState.back.passOp = VK_STENCIL_OP_KEEP;
		//depthStencilState.back.compareOp = VK_COMPARE_OP_ALWAYS;
		//depthStencilState.stencilTestEnable = VK_FALSE;
		//depthStencilState.front = depthStencilState.back;

		VkPipelineMultisampleStateCreateInfo multisampleState = {};
		multisampleState.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		multisampleState.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
		multisampleState.pSampleMask = nullptr;

		uint32_t vertexFormat = mSpecification.vertexFormat;
		uint32_t sizeOfVertex = 0;
		uint32_t posOffset = 0;
		uint32_t colOffset = 0;
		uint32_t uvOffset = 0;
		uint32_t normalOffset = 0;
		uint32_t tangentOffset = 0;
		if (vertexFormat == VertexP::Format)
		{
			sizeOfVertex = sizeof(VertexP);
			posOffset = offsetof(VertexP, position);
		}
		else if (vertexFormat == VertexPC::Format)
		{
			sizeOfVertex = sizeof(VertexPC);
			posOffset = offsetof(VertexPC, position);
			colOffset = offsetof(VertexPC, color);
		}
		else if (vertexFormat == VertexPosColTex::Format)
		{
			sizeOfVertex = sizeof(VertexPosColTex);
			posOffset = offsetof(VertexPosColTex, position);
			colOffset = offsetof(VertexPosColTex, color);
			uvOffset = offsetof(VertexPosColTex, texCoord);

		}
		else if (vertexFormat == VertexPT::Format)
		{
			sizeOfVertex = sizeof(VertexPT);
			posOffset = offsetof(VertexPT, position);
			tangentOffset = offsetof(VertexPT, tangent);
		}
		else if (vertexFormat == Vertex::Format)
		{
			sizeOfVertex = sizeof(Vertex);
			posOffset = offsetof(Vertex, position);
			normalOffset = offsetof(Vertex, normal);
			tangentOffset = offsetof(Vertex, tangent);
			uvOffset = offsetof(Vertex, texCoord);
		}
		else if (vertexFormat == VertexBone::Format)
		{
			sizeOfVertex = sizeof(VertexBone);
			posOffset = offsetof(VertexBone, position);
			normalOffset = offsetof(VertexBone, normal);
			tangentOffset = offsetof(VertexBone, tangent);
			uvOffset = offsetof(VertexBone, texCoord);
		}

		VkVertexInputBindingDescription vertexInputBinding = {};
		vertexInputBinding.binding = 0;
		vertexInputBinding.stride = (uint32_t)sizeOfVertex;
		vertexInputBinding.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

		uint32_t location = 0;
		std::vector<VkVertexInputAttributeDescription> vertexInputAttributes;
		if (vertexFormat & VF_POSITION)
		{
			VkVertexInputAttributeDescription vertexAttributeDesc = {};
			vertexAttributeDesc.binding = 0;
			vertexAttributeDesc.location = location++;
			vertexAttributeDesc.format = VK_FORMAT_R32G32B32_SFLOAT;
			vertexAttributeDesc.offset = posOffset;
			vertexInputAttributes.push_back(vertexAttributeDesc);
		}
		if (vertexFormat & VF_COLOR)
		{
			VkVertexInputAttributeDescription vertexAttributeDesc = {};
			vertexAttributeDesc.binding = 0;
			vertexAttributeDesc.location = location++;
			vertexAttributeDesc.format = VK_FORMAT_R32G32B32A32_SFLOAT;
			vertexAttributeDesc.offset = colOffset;
			vertexInputAttributes.push_back(vertexAttributeDesc);
		}
		if (vertexFormat & VF_NORMAL)
		{
			VkVertexInputAttributeDescription vertexAttributeDesc = {};
			vertexAttributeDesc.binding = 0;
			vertexAttributeDesc.location = location++;
			vertexAttributeDesc.format = VK_FORMAT_R32G32B32_SFLOAT;
			vertexAttributeDesc.offset = normalOffset;
			vertexInputAttributes.push_back(vertexAttributeDesc);
		}
		if (vertexFormat & VF_TANGENT)
		{
			VkVertexInputAttributeDescription vertexAttributeDesc = {};
			vertexAttributeDesc.binding = 0;
			vertexAttributeDesc.location = location++;
			vertexAttributeDesc.format = VK_FORMAT_R32G32B32_SFLOAT;
			vertexAttributeDesc.offset = tangentOffset;
			vertexInputAttributes.push_back(vertexAttributeDesc);
		}

		if (vertexFormat & VF_UV)
		{
			VkVertexInputAttributeDescription vertexAttributeDesc = {};
			vertexAttributeDesc.binding = 0;
			vertexAttributeDesc.location = location++;
			vertexAttributeDesc.format = VK_FORMAT_R32G32_SFLOAT;
			vertexAttributeDesc.offset = uvOffset;
			vertexInputAttributes.push_back(vertexAttributeDesc);
		}

		VkPipelineVertexInputStateCreateInfo vertexInputState = {};
		vertexInputState.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		vertexInputState.vertexBindingDescriptionCount = 1;
		vertexInputState.pVertexBindingDescriptions = &vertexInputBinding;
		vertexInputState.vertexAttributeDescriptionCount = static_cast<uint32_t>(vertexInputAttributes.size());
		vertexInputState.pVertexAttributeDescriptions = vertexInputAttributes.data();

		std::vector<VkPipelineShaderStageCreateInfo> shaderStages =  shader->getShaderStages();
		pipelineCreateInfo.stageCount = static_cast<uint32_t>(shaderStages.size());
		pipelineCreateInfo.pStages = shaderStages.data();

		pipelineCreateInfo.pVertexInputState = &vertexInputState;
		pipelineCreateInfo.pInputAssemblyState = &inputAssemblyState;
		pipelineCreateInfo.pRasterizationState = &rasterizationState;
		pipelineCreateInfo.pColorBlendState = &colorBlendState;
		pipelineCreateInfo.pMultisampleState = &multisampleState;
		pipelineCreateInfo.pViewportState = &viewportState;
		pipelineCreateInfo.pDepthStencilState = &depthStencilState;
		//get renderpass from frame buffer;
		pipelineCreateInfo.renderPass = (swapchain->getRenderpass()->getRenderPass());// ?
		pipelineCreateInfo.pDynamicState = &dynamicState;

		//should store this cache onto disk, if it isnt found, do all the above, otherwise quick load from cache.
		//preferably, all these should be merged using vkMergePipelineCache written into one file, then on load, split out to multiple threads to load.
		VkPipelineCacheCreateInfo pipelineCacheCreateInfo = {};
		pipelineCacheCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
		ASSERT(vkCreatePipelineCache(device->getLogicalDevice(), &pipelineCacheCreateInfo, nullptr, &mPipelineCache) == VK_SUCCESS, "[SERenderer::VulkanPipeline] Failed creation of pipeline cache.");

		ASSERT(vkCreateGraphicsPipelines(device->getLogicalDevice(), mPipelineCache, 1, &pipelineCreateInfo, nullptr, &mPipeline) == VK_SUCCESS, "[SERenderer::VulkanPipeline] Failed creation of pipeline.");

	}

	void VulkanPipeline::bind()
	{
		auto* swapchain = dynamic_cast<VulkanSwapChain*>(Renderer::Get()->getSwapchain());
		VkExtent2D surfaceExtent = swapchain->getSurfaceExtent();
		VkCommandBuffer cmdBuffer = swapchain->getCurrentCommandBuffer();

		vkCmdBindPipeline(cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, mPipeline);
	}

	void VulkanPipeline::pushConstant(VkShaderStageFlagBits shaderStage, size_t offset, size_t size, void* data)
	{
		ASSERT(data, "[SERenderer::VulkanPipeline] Invalid Data passed into function.");
		auto* swapchain = dynamic_cast<VulkanSwapChain*>(Renderer::Get()->getSwapchain());
		VkCommandBuffer cmdBuffer = swapchain->getCurrentCommandBuffer();

		vkCmdPushConstants(cmdBuffer, mPipelineLayout, shaderStage, (uint32_t)offset, (uint32_t)size, data);
	}

}

