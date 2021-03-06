#pragma once
#include "Common.h"
#include "VulkanImage.h"
#include "VulkanImageView.h"
#include "VulkanMemoryObject.h"
namespace SingularityEngine::Graphics
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
		VulkanMemoryObject* mMemoryObject = nullptr;
		VulkanImage* mImage = nullptr;
		VulkanImageView* mImageView = nullptr;
	};
}