#pragma once
#include "Common.h"
#include "Image.h"
#include "ImageView.h"
#include "MemoryObject.h"
namespace SingularityEngine::Vulkan
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
		VkImageView getImageView() { return mImageView->get(); }
		~SampledImage();
	private:
		MemoryObject* mMemoryObject = nullptr;
		Image* mImage = nullptr;
		ImageView* mImageView = nullptr;
	};
}
