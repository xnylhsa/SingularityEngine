#include "Precompiled.h"
#include "Sampler.h"
#include "VulkanFunctions.h"


using namespace SingularityEngine::Graphics;

Sampler::Sampler(VkDevice device, Sampler::SamplerParams params)
{
	VkSamplerCreateInfo samplerCreateInfo = {
		VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO,
		nullptr,
		0,
		params.magFilter,
		params.minFilter,
		params.mipMapMode,
		params.uAddressMode,
		params.vAddressMode,
		params.wAddressMode,
		params.lodBias,
		params.anisotropyEnable,
		params.maxAnisotropy,
		params.compareEnable,
		params.compareOperator,
		params.minLOD,
		params.maxLOD,
		params.borderColor,
		params.unnormalizedCoords
	};
	ASSERT(vkCreateSampler(device, &samplerCreateInfo, nullptr, &mSampler) == VK_SUCCESS, "[Graphics::Sampler] Failed Creation!");
}

bool Sampler::destroy(VkDevice device)
{
	if (mSampler != VK_NULL_HANDLE)
	{
		vkDestroySampler(device, mSampler, nullptr);
		mSampler = VK_NULL_HANDLE;
	}
	return true;
}

Sampler::~Sampler()
{
	ASSERT(mSampler == VK_NULL_HANDLE, "[Graphics::Sampler] vulkan sampler was never destroyed!");
}
