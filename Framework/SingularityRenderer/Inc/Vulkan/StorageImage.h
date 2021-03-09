#pragma once
#include "Common.h"
#include "Vulkan/Texture/VulkanImage.h"
#include "Vulkan/Texture/VulkanImageView.h"
#include "Vulkan/Memory/VulkanMemoryObject.h"
namespace SingularityEngine::SERenderer
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