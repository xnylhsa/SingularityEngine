#pragma once
#include "VulkanImage.h"
#include "VulkanImageView.h"
#include "VulkanMemoryObject.h"
namespace SingularityEngine::Graphics
{

	class ImageCubeMap
	{
	public:
		ImageCubeMap(VkPhysicalDevice physicalDevice, VkDevice device, VkExtent2D size, uint32_t numMipMaps, VkImageUsageFlags usage, VkImageAspectFlags aspect);
		bool destroy(VkDevice device);
		~ImageCubeMap();
	private:
		VulkanMemoryObject* mMemoryObject = nullptr;
		VulkanImage* mImage = nullptr;
		VulkanImageView* mImageView = nullptr;
	};

}