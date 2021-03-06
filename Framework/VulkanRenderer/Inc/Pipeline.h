//#ifndef SINGULARITY_ENGINE_VULKAN_PIPELINE
//#define SINGULARITY_ENGINE_VULKAN_PIPELINE
//#include "Common.h"
//#include "ShaderModule.h"
//namespace SingularityEngine::Graphics
//{
//	enum class ShaderStages
//	{
//		VERTEX = 1,
//		TESSELLATION_CONTROL = 2,
//		TESSELLATION_EVALUATION = 4,
//		GEOMETRY = 8,
//		FRAGMENT = 16,
//		COMPUTE = 32,
//		TASK = 64,
//		MESH = 128,
//		RTX_RAYGEN = 256,
//		RTX_ANY_HIT = 512,
//		RTX_CLOSEST_HIT = 1024,
//		RTX_MISS = 2048,
//		RTX_INTERSECTION = 4096,
//		RTX_CALLABLE = 8192,
//		TOTAL
//	};
//
//	struct RasterizationInfo
//	{
//		VkPipelineRasterizationStateCreateFlags    flags;
//		VkBool32                                   depthClampEnable;
//		VkBool32                                   rasterizerDiscardEnable;
//		VkPolygonMode                              polygonMode;
//		VkCullModeFlags                            cullMode;
//		VkFrontFace                                frontFace;
//		VkBool32 depthBiasEnable;
//		float depthBiasConstantFactor;
//		float depthBiasClamp;
//		float depthBiasSlopeFactor;
//		float lineWidth;
//	};
//	struct MultiSampleInfo
//	{
//		VkSampleCountFlagBits                    rasterizationSamples;
//		VkBool32                                 sampleShadingEnable;
//		float                                    minSampleShading;
//		const VkSampleMask* pSampleMask;
//		VkBool32                                 alphaToCoverageEnable;
//		VkBool32                                 alphaToOneEnable;
//	};
//
//	struct DepthStencilInfo
//	{
//		VkBool32                                  depthTestEnable;
//		VkBool32                                  depthWriteEnable;
//		VkCompareOp                               depthCompareOp;
//		VkBool32                                  depthBoundsTestEnable;
//		VkBool32                                  stencilTestEnable;
//		VkStencilOpState                          front;
//		VkStencilOpState                          back;
//		float                                     minDepthBounds;
//		float                                     maxDepthBounds;
//	};
//
//	struct ShaderStageInfo
//	{
//		ShaderStages stage;
//		ShaderModule* shaderModule;
//		std::string entrypoint;
//	};
//
//	class Pipeline
//	{
//	public:
//		Pipeline(VkDevice device, std::vector<ShaderStageInfo> pipelineShaderStages, uint32_t vertexFormat, VkPrimitiveTopology topology, VkBool32 restartIndexEnable, bool useTesselation, uint32_t patchControlPoints, std::vector<Math::Rectangle> viewports, RasterizationInfo rasterizationInfo, MultiSampleInfo multisampleInfo, DepthStencilInfo depthStencilInfo, std::vector<VkDynamicState> dynamicStates, std::vector<VkDescriptorSetLayout> const& descriptor_set_layouts, std::vector<VkPushConstantRange> const& push_constant_ranges);
//	private:
//		std::vector<VkPipelineShaderStageCreateInfo> acquirePipelineShaderStageCreateInfos(std::vector<ShaderStageInfo> pipelineShaderStages);
//		bool createPipelineCache(std::vector<unsigned char> cacheData, VkDevice device);
//		bool retrievePipelineCache(VkDevice device, std::vector<unsigned char>  pipelineCacheData);
//		bool mergePipelineCaches(VkDevice device, std::vector<VkPipelineCache> sourcePipelineCaches);
//		VkPipelineLayout mPipelineLayout;
//		VkPipelineCache mPipelineCache;
//	};
//}
//
//#endif
//
//
