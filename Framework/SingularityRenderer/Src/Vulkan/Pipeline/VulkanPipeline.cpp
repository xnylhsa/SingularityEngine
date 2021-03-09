#include "Precompiled.h"
//#include "Pipeline.h"
//#include "VulkanFunctions.h"
//#include "VertexTypes.h"
//
//namespace
//{
//	std::vector<VkVertexInputAttributeDescription> GetVertexDescription(uint32_t vertexFormat, uint32_t& totalSizeOut)
//	{
//		std::vector<VkVertexInputAttributeDescription> desc;
//		uint32_t currentOffset = 0;
//		uint32_t currentLocation = 0;
//		if (vertexFormat & VF_POSITION)
//		{
//			desc.push_back({ currentLocation, 0,  VK_FORMAT_R32G32B32_SFLOAT, currentOffset});
//			currentOffset += 3 * sizeof(float);
//			currentLocation++;
//		}
//		if (vertexFormat & VF_NORMAL)
//		{
//			desc.push_back({ currentLocation, 0, VK_FORMAT_R32G32B32_SFLOAT, currentOffset });
//			currentOffset += 3 * sizeof(float);
//			currentLocation++;
//		}
//		if (vertexFormat & VF_TANGENT)
//		{
//			desc.push_back({ currentLocation, 0 , VK_FORMAT_R32G32B32_SFLOAT, currentOffset });
//			currentOffset += 3 * sizeof(float);
//			currentLocation++;
//		}
//		if (vertexFormat & VF_COLOR)
//		{
//			desc.push_back({ currentLocation, 0, VK_FORMAT_R32G32B32A32_SFLOAT, currentOffset });
//			currentOffset += 4 * sizeof(float);
//			currentLocation++;
//		}
//		if (vertexFormat & VF_UV)
//		{
//			desc.push_back({ currentLocation, 0, VK_FORMAT_R32G32_SFLOAT, currentOffset });
//			currentOffset += 2 * sizeof(float);
//			currentLocation++;
//		}
//		if (vertexFormat & VF_BINDEX)
//		{
//			desc.push_back({ currentLocation, 0 , VK_FORMAT_R32G32B32A32_UINT, currentOffset });
//			currentOffset += 4 * sizeof(uint32_t);
//			currentLocation++;
//		}
//		if (vertexFormat & VF_BWEIGHT)
//		{
//			desc.push_back({ currentLocation, 0 , VK_FORMAT_R32G32B32A32_SFLOAT, currentOffset });
//			currentOffset += 4 * sizeof(float);
//			currentLocation++;
//		}
//		totalSizeOut = currentOffset;
//		return desc;
//	}
//
//	VkShaderStageFlagBits ConvertToVKShaderStageFlag(SingularityEngine::Graphics::ShaderStages shaderStage)
//	{
//		return static_cast<VkShaderStageFlagBits>(shaderStage);
//	}
//}
//
//using namespace SingularityEngine::Graphics;
//
//Pipeline::Pipeline(VkDevice device, std::vector<ShaderStageInfo> pipelineShaderStages, uint32_t vertexFormat, VkPrimitiveTopology topology, VkBool32 restartIndexEnable, bool useTesselation, uint32_t patchControlPoints, std::vector<Math::Rectangle> viewports, RasterizationInfo rasterizationInfo, MultiSampleInfo multisampleInfo, DepthStencilInfo depthStencilInfo, std::vector<VkDynamicState> dynamicStates, std::vector<VkDescriptorSetLayout> const& descriptor_set_layouts, std::vector<VkPushConstantRange> const& push_constant_ranges, VkPipelineCreateFlags additionalOptions, VkRenderPass renderpass, uint32_t subpass)
//{
//	std::vector<VkPipelineShaderStageCreateInfo> shaderStageCreateInfos = acquirePipelineShaderStageCreateInfos(pipelineShaderStages);
//	uint32_t totalVertexFormatSize = 0;
//	std::vector<VkVertexInputAttributeDescription> vertexAttributeDescriptions = GetVertexDescription(vertexFormat, totalVertexFormatSize);
//	std::vector<VkVertexInputBindingDescription> bindingDescriptions = {
//		{
//			0,
//			totalVertexFormatSize,
//			VK_VERTEX_INPUT_RATE_VERTEX
//		}
//	};
//
//	VkPipelineVertexInputStateCreateInfo vertexInputStateCreateInfo = {
//		VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
//		nullptr,
//		0,
//		static_cast<uint32_t>(bindingDescriptions.size()),
//		bindingDescriptions.data(),
//		static_cast<uint32_t>(vertexAttributeDescriptions.size()),
//		vertexAttributeDescriptions.data()
//	};
//
//	VkPipelineInputAssemblyStateCreateInfo inputAssemblyStateCreateInfo = {
//		VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
//		nullptr,
//		0,
//		topology,
//		restartIndexEnable
//	};
//
//
//	VkPipelineTessellationStateCreateInfo tesselationStateCreateInfo = {
//		VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_STATE_CREATE_INFO,
//		nullptr,
//		0,
//		patchControlPoints
//	};
//
//	std::vector<VkViewport> viewPortInfos(viewports.size());
//	std::vector<VkRect2D> scissors(viewports.size());
//
//	for (size_t i = 0; i < viewports.size(); i++)
//	{
//		VkViewport& vkViewport = viewPortInfos[i];
//		Math::Rectangle viewport = viewports[i];
//		VkRect2D scissor = scissors[i];
//		Math::Vector2 max = viewport.GetMax();
//		vkViewport.height = viewport.size.y * 2;
//		vkViewport.width = viewport.size.x * 2;
//		vkViewport.maxDepth = 1.0f;
//		vkViewport.minDepth = 0.0f;
//		Math::Vector2 min = viewport.GetMin();
//		vkViewport.x = min.x;
//		vkViewport.y = min.y;
//		scissor.extent.height = viewport.size.y * 2;
//		scissor.extent.width = viewport.size.x * 2;
//		scissor.offset.x = min.x;
//		scissor.offset.y = min.y;
//	}
//
//	VkPipelineViewportStateCreateInfo viewportCreateInfo = {
//		VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
//		nullptr,
//		0,
//		static_cast<uint32_t>(viewPortInfos.size()),
//		viewPortInfos.data(),
//		static_cast<uint32_t>(scissors.size()),
//		scissors.data()
//	};
//
//	VkPipelineRasterizationStateCreateInfo rasterizationStateCreateInfo = {
//	  VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
//	  nullptr,
//	  0,
//	  rasterizationInfo.depthClampEnable,
//	  rasterizationInfo.rasterizerDiscardEnable,
//	  rasterizationInfo.polygonMode,
//	  rasterizationInfo.cullMode,
//	  rasterizationInfo.frontFace,
//	  rasterizationInfo.depthBiasEnable,
//	  rasterizationInfo.depthBiasConstantFactor,
//	  rasterizationInfo.depthBiasClamp,
//	  rasterizationInfo.depthBiasSlopeFactor,
//	  rasterizationInfo.lineWidth
//	};
//
//	VkPipelineMultisampleStateCreateInfo multisampleCreateInfo = {
//	  VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
//	  nullptr,
//	  0,
//	  multisampleInfo.rasterizationSamples,
//	  multisampleInfo.sampleShadingEnable,
//	  multisampleInfo.minSampleShading,
//	  multisampleInfo.pSampleMask,
//	  multisampleInfo.alphaToCoverageEnable,
//	  multisampleInfo.alphaToOneEnable
//	};
//
//	VkPipelineDepthStencilStateCreateInfo depthStencilStateCreateInfo = {
//		VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO,
//		nullptr,
//		0,
//		depthStencilInfo.depthTestEnable,
//		depthStencilInfo.depthWriteEnable,
//		depthStencilInfo.depthCompareOp,
//		depthStencilInfo.depthBoundsTestEnable,
//		depthStencilInfo.stencilTestEnable,
//		depthStencilInfo.front,
//		depthStencilInfo.back,
//		depthStencilInfo.minDepthBounds,
//		depthStencilInfo.maxDepthBounds,
//	};
//
//	std::vector<VkPipelineColorBlendAttachmentState> attachmentBlendStates = {
//		{
//			false,
//			VK_BLEND_FACTOR_ONE,
//			VK_BLEND_FACTOR_ONE,
//			VK_BLEND_OP_ADD,
//			VK_BLEND_FACTOR_ONE,
//			VK_BLEND_FACTOR_ONE,
//			VK_BLEND_OP_ADD,
//			VK_COLOR_COMPONENT_R_BIT |
//			VK_COLOR_COMPONENT_G_BIT |
//			VK_COLOR_COMPONENT_B_BIT |
//			VK_COLOR_COMPONENT_A_BIT
//		}
//	};
//
//	VkPipelineColorBlendStateCreateInfo blendStateCreateInfo = {
//		VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
//		nullptr,
//		0,
//		VK_FALSE,
//		VK_LOGIC_OP_COPY,
//		static_cast<uint32_t>(attachmentBlendStates.size()),
//		attachmentBlendStates.data(),
//		{
//		1.0f,
//		1.0f,
//		1.0f,
//		1.0f
//		}
//	};
//
//	VkPipelineDynamicStateCreateInfo dynamicStateCreateInfo = {
//		VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
//		nullptr,
//		0,
//		static_cast<uint32_t>(dynamicStates.size()),
//		dynamicStates.data()
//	};
//
//	VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo = {
//		VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
//		nullptr,
//		0,
//		static_cast<uint32_t>(descriptor_set_layouts.size()),
//		descriptor_set_layouts.data(),
//		static_cast<uint32_t>(push_constant_ranges.size()),
//		push_constant_ranges.data()
//	};
//
//	ASSERT(vkCreatePipelineLayout(device, &pipelineLayoutCreateInfo, nullptr, &mPipelineLayout) == VK_SUCCESS, "[Graphics::Pipeline]Could not create pipeline layout.");
//
//	VkGraphicsPipelineCreateInfo graphics_pipeline_create_info = {
//		VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
//		nullptr,
//		additionalOptions,
//		static_cast<uint32_t>(shaderStageCreateInfos.size()),
//		shaderStageCreateInfos.data(),
//		&vertexInputStateCreateInfo,
//		&inputAssemblyStateCreateInfo,
//		&tesselationStateCreateInfo,
//		&viewportCreateInfo,
//		&rasterizationStateCreateInfo,
//		&multisampleCreateInfo,
//		&depthStencilStateCreateInfo,
//		&blendStateCreateInfo,
//		&dynamicStateCreateInfo,
//		mPipelineLayout,
//		renderpass,
//		subpass,
//		VK_NULL_HANDLE,
//		-1
//	};
//
//	mGraphicsPipelines.resize(graphics_pipeline_create_infos.size());
//	VkResult result = vkCreateGraphicsPipelines(device, mPipelineCache, static_cast<uint32_t>(graphics_pipeline_create_infos.size()), graphics_pipeline_create_infos.data(), nullptr, graphics_pipelines.data());
//	if (VK_SUCCESS != result) {
//		std::cout << "Could not create a graphics pipeline." << std::endl;
//		return false;
//	}
//	return true;
//}
//
//std::vector<VkPipelineShaderStageCreateInfo> Pipeline::acquirePipelineShaderStageCreateInfos(std::vector<ShaderStageInfo> pipelineShaderStages)
//{
//	std::vector<VkPipelineShaderStageCreateInfo> shaderStageCreateInfos;
//	shaderStageCreateInfos.resize(pipelineShaderStages.size());
//	for (size_t i = 0; i < pipelineShaderStages.size(); i++)
//	{
//		VkPipelineShaderStageCreateInfo& createInfo = shaderStageCreateInfos[i];
//		ShaderStageInfo& shaderStageInfo = pipelineShaderStages[i];
//		createInfo.stage = ConvertToVKShaderStageFlag(shaderStageInfo.stage);
//		createInfo.module = shaderStageInfo.shaderModule->get();
//		createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
//		createInfo.pName = shaderStageInfo.entrypoint.c_str();
//		createInfo.pNext = nullptr;
//		createInfo.pSpecializationInfo = nullptr;
//		createInfo.flags = 0;
//	}
//	return shaderStageCreateInfos;
//}
//
//bool Pipeline::createPipelineCache(std::vector<unsigned char> cacheData, VkDevice device)
//{
//	VkPipelineCacheCreateInfo pipeline_cache_create_info = {
//		VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO,
//		nullptr,
//		0,
//		static_cast<uint32_t>(cacheData.size()),
//		cacheData.data()
//	};
//
//	VkResult result = vkCreatePipelineCache(device, &pipeline_cache_create_info, nullptr, &mPipelineCache);
//	if (VK_SUCCESS != result) {
//		LOG("[Graphics::Pipeline] Could not create Cache!");
//		return false;
//	}
//	return true;
//}
//
//bool Pipeline::retrievePipelineCache(VkDevice device, std::vector<unsigned char> pipelineCacheData)
//{
//	size_t dataSize = 0;
//	VkResult result = VK_SUCCESS;
//
//	result = vkGetPipelineCacheData(device, mPipelineCache, &dataSize, nullptr);
//	if ((VK_SUCCESS != result) ||
//		(0 == dataSize)) {
//		LOG("Could not get the size of the pipeline cache.");
//		return false;
//	}
//	pipelineCacheData.resize(dataSize);
//}
//
//bool Pipeline::mergePipelineCaches(VkDevice device, std::vector<VkPipelineCache> sourcePipelineCaches)
//{
//	VkResult result = vkMergePipelineCaches(device, mPipelineCache, static_cast<uint32_t>(sourcePipelineCaches.size()), sourcePipelineCaches.data());
//	if (VK_SUCCESS != result) {
//		LOG("Could not merge pipeline cache objects.");
//		return false;
//	}
//	return true;
//}
