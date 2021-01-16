#include "Precompiled.h"
#include "SampledImage.h"
#include "VulkanFunctions.h"

using namespace SingularityEngine::Vulkan;

SampledImage::SampledImage(VkPhysicalDevice physicalDevice, VkDevice device, SampledImage::SampleImageParams params)
{
	VkFormatProperties formatProperties;
	vkGetPhysicalDeviceFormatProperties(physicalDevice, params.format, &formatProperties);
	if (!(formatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT)) {
		LOG("[Graphics::SampledImage] Provided format is not supported for a sampled image!");
		return;
	}
	if (params.linearFiltering &&
		!(formatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT)) {
		LOG("[Graphics::SampledImage] Provided format is not supported for linear image filtering!");
		return;
	}

	mImage = new Image(device, params.type, params.format, { params.size.width, params.size.height, 1 }, params.numMipMaps, params.numLayers, VK_SAMPLE_COUNT_1_BIT, params.usage | VK_IMAGE_USAGE_SAMPLED_BIT, false);
	ASSERT(mImage->isValid(), "[Graphics::SampledImage] Failed to create image!");
	mMemoryObject = new MemoryObject();
	ASSERT(mMemoryObject->createImageMemoryObject(physicalDevice, device, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, mImage->getImage()), "[Graphics::Image2D] Failed to allocate and bind memory object");
	mImageView = new ImageView(device, mImage->getImage(), params.viewType, params.format, params.aspect);
}

bool SampledImage::destroy(VkDevice device)
{
	return mImageView->destroy(device) && mMemoryObject->destroy(device) && mImage->destroy(device);
}

SampledImage::~SampledImage()
{
	SafeDelete(mImageView);
	SafeDelete(mImage);
	SafeDelete(mMemoryObject);
}
