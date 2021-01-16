#pragma once
#include "Common.h"
#include "Image.h"
#include "ImageView.h"
#include "MemoryObject.h"
namespace SingularityEngine::Vulkan
{
	class Image2D
	{
	public:
		Image2D(VkPhysicalDevice physicalDevice, VkDevice device, VkFormat format, VkExtent2D size, uint32_t numMipMaps, uint32_t numLayers, VkSampleCountFlagBits samples, VkImageUsageFlags usage, VkImageAspectFlags aspect);
		bool destroy(VkDevice device);
		~Image2D();
	private:
		MemoryObject* mMemoryObject = nullptr;
		Image* mImage = nullptr;
		ImageView* mImageView = nullptr;
	}
}