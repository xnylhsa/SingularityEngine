#pragma once
#include "Vulkan/Texture/VulkanImage.h"
#include "Vulkan/Texture/VulkanImageView.h"
#include "Vulkan/Memory/VulkanMemoryObject.h"
namespace SingularityEngine::SERenderer
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