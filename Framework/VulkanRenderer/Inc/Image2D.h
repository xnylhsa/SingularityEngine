#pragma once
#include "Common.h"
#include "VulkanImage.h"
#include "VulkanImageView.h"
#include "VulkanMemoryObject.h"
namespace SingularityEngine::Graphics
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