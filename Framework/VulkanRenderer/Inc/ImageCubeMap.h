#pragma once
#include "Image.h"
#include "ImageView.h"
#include "MemoryObject.h"
namespace SingularityEngine::Vulkan
{

	class ImageCubeMap
	{
	public:
		ImageCubeMap(VkPhysicalDevice physicalDevice, VkDevice device, VkExtent2D size, uint32_t numMipMaps, VkImageUsageFlags usage, VkImageAspectFlags aspect);
		bool destroy(VkDevice device);
		~ImageCubeMap();
	private:
		MemoryObject* mMemoryObject = nullptr;
		Image* mImage = nullptr;
		ImageView* mImageView = nullptr;
	};

}