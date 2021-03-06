#pragma once
#include "Common.h"
#include "VulkanImage.h"
#include "VulkanImageView.h"
#include "VulkanMemoryObject.h"
namespace SingularityEngine::Graphics
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
