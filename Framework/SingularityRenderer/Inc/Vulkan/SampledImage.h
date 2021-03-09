#pragma once
#include "Common.h"
#include "Vulkan/Texture/VulkanImage.h"
#include "Vulkan/Texture/VulkanImageView.h"
#include "Vulkan/Memory/VulkanMemoryObject.h"
namespace SingularityEngine::SERenderer
{
	class SampledImage
	{
	public:
		struct SampleImageParams
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
			bool linearFiltering;
		};
		SampledImage(VkPhysicalDevice physicalDevice, VkDevice device, SampleImageParams params);
		bool destroy(VkDevice device);
		VkImageView getImageView() { return *mImageView; }
		~SampledImage();
	private:
		VulkanMemoryObject* mMemoryObject = nullptr;
		VulkanImage* mImage = nullptr;
		VulkanImageView* mImageView = nullptr;
	};
}
