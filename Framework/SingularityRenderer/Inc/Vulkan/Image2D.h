#pragma once
#include "Common.h"
#include "Vulkan/Texture/VulkanImage.h"
#include "Vulkan/Texture/VulkanImageView.h"
#include "Vulkan/Memory/VulkanMemoryObject.h"
namespace SingularityEngine::SERenderer
{
	class Image2D
	{
	public:
		Image2D(VkPhysicalDevice physicalDevice, VkDevice device, VkFormat format, VkExtent2D size, uint32_t numMipMaps, uint32_t numLayers, VkSampleCountFlagBits samples, VkImageUsageFlags usage, VkImageAspectFlags aspect);
		bool destroy(VkDevice device);
		~Image2D();
	private:
		VulkanMemoryObject* mMemoryObject = nullptr;
		VulkanImage* mImage = nullptr;
		VulkanImageView* mImageView = nullptr;
	};
}