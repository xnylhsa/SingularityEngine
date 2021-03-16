#ifndef SINGULARITY_ENGINE_VULKAN_SAMPLER
#define SINGULARITY_ENGINE_VULKAN_SAMPLER
#include "Common.h"

namespace SingularityEngine::SERenderer
{
	enum class SamplerTypes
	{
		NearestClamp = 0,
		LinearClamp,
		TrilinearClamp,
		NearestWrap,
		LinearWrap,
		TrilinearWrap,
		NearestShadow,
		LinearShadow,
		Count
	};


	struct SamplerCreateInfo
	{
		VkFilter magFilter = VK_FILTER_MAX_ENUM;
		VkFilter minFilter = VK_FILTER_MAX_ENUM;
		VkSamplerMipmapMode mipMapMode = VK_SAMPLER_MIPMAP_MODE_MAX_ENUM;
		VkSamplerAddressMode addressModeU = VK_SAMPLER_ADDRESS_MODE_MAX_ENUM;
		VkSamplerAddressMode addressModeV = VK_SAMPLER_ADDRESS_MODE_MAX_ENUM;
		VkSamplerAddressMode addressModeW = VK_SAMPLER_ADDRESS_MODE_MAX_ENUM;
		float mipLODBias = 0.0f;
		VkBool32 anisotropyEnable = VK_FALSE;
		float maxAnisotropy = 0.0f;
		VkBool32 compareEnable = VK_FALSE;
		VkCompareOp compareOp = VK_COMPARE_OP_MAX_ENUM;
		float minLOD = 0.0f;
		float maxLOD = 1.0f;
		VkBorderColor borderColor = VK_BORDER_COLOR_MAX_ENUM;
		VkBool32 unnormalizedCoords = VK_FALSE;

	};

	class VulkanSampler
	{
	public:
		VulkanSampler(SamplerCreateInfo createInfo);
		~VulkanSampler();

		VkSampler getSampler() { return mSampler; }
		const SamplerCreateInfo& getCreateInfo() { return mCreateInfo; }
	private:

		VkSamplerCreateInfo convertCreateInfoToVK();
		VkSampler mSampler;
		SamplerCreateInfo mCreateInfo;
	};

}


#endif