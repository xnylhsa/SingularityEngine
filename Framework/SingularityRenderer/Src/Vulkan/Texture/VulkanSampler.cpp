#include "Precompiled.h"
#include "vulkan/Texture/VulkanSampler.h"
#include "vulkan/Device/VulkanDevice.h"
#include "RendererAPI/Renderer.h"
#include "vulkan/Util/VulkanFunctions.h"


namespace SingularityEngine::SERenderer
{

	VkSamplerCreateInfo VulkanSampler::convertCreateInfoToVK()
	{
		VkSamplerCreateInfo info = { VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO };

		info.magFilter = mCreateInfo.magFilter;
		info.minFilter = mCreateInfo.minFilter;
		info.mipmapMode = mCreateInfo.mipMapMode;
		info.addressModeU = mCreateInfo.addressModeU;
		info.addressModeV = mCreateInfo.addressModeV;
		info.addressModeW = mCreateInfo.addressModeW;
		info.mipLodBias = mCreateInfo.mipLODBias;
		info.anisotropyEnable = mCreateInfo.anisotropyEnable;
		info.maxAnisotropy = mCreateInfo.maxAnisotropy;
		info.compareEnable = mCreateInfo.compareEnable;
		info.compareOp = mCreateInfo.compareOp;
		info.minLod = mCreateInfo.minLOD;
		info.maxLod = mCreateInfo.maxLOD;
		info.borderColor = mCreateInfo.borderColor;
		info.unnormalizedCoordinates = mCreateInfo.unnormalizedCoords;
		return info;
	}

	VulkanSampler::VulkanSampler(SamplerCreateInfo createInfo)
	{
		mCreateInfo = createInfo;
		std::shared_ptr<VulkanDevice> device = std::dynamic_pointer_cast<VulkanDevice>(Renderer::Get()->getGraphicsDevice());

		auto info = convertCreateInfoToVK();

		if (vkCreateSampler(device->getLogicalDevice(), &info, nullptr, &mSampler) != VK_SUCCESS)
			LOG("[SErenderer::VulkanSampler] Creation failed");
	}

	VulkanSampler::~VulkanSampler()
	{
		if (mSampler)
		{
			std::shared_ptr<VulkanDevice> device = std::dynamic_pointer_cast<VulkanDevice>(Renderer::Get()->getGraphicsDevice());

			vkDestroySampler(device->getLogicalDevice(), mSampler, nullptr);
		}
	}

}
