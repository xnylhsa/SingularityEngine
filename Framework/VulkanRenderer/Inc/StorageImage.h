#pragma once
#include "Common.h"
#include "Image.h"
#include "ImageView.h"
#include "MemoryObject.h"
namespace SingularityEngine::Vulkan
{
	class StorageImage
	{
	public:
		struct StorageImageParams
		{
			VkImageType type;
			VkFormat format;
			VkExtent3D size;
			uint32_t numMipMaps;
			uint32_t numLayers;
			bool cubemap;
			VkImageViewType viewType;
			VkImageUsageFlags usage;
			VkImageAspectFlags aspect;
			bool atomicOperations;
		};
		StorageImage(VkPhysicalDevice physicalDevice, VkDevice device, StorageImageParams params);
		bool destroy(VkDevice device);
		~StorageImage();
	private:
		MemoryObject* mMemoryObject = nullptr;
		Image* mImage = nullptr;
		ImageView* mImageView = nullptr;
	}
}