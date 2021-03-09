#include "Precompiled.h"
//#include "StorageImage.h"
//#include "VulkanFunctions.h"
//
//using namespace SingularityEngine::Graphics;
//
//StorageImage::StorageImage(VkPhysicalDevice physicalDevice, VkDevice device, StorageImage::StorageImageParams params)
//{
//	VkFormatProperties formatProperties;
//	vkGetPhysicalDeviceFormatProperties(physicalDevice, params.format, &formatProperties);
//	if (!(formatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT)) {
//		LOG("[Graphics::StorageImage] Provided format is not supported for a sampled image!");
//		return;
//	}
//	if (params.atomicOperations &&
//		!(formatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_STORAGE_IMAGE_ATOMIC_BIT)) {
//		LOG("[Graphics::StorageImage] Provided format is not supported for atomic operations filtering!");
//		return;
//	}
//
//	mImage = new VulkanImage(device, params.type, params.format, params.size, params.numMipMaps, params.numLayers, VK_SAMPLE_COUNT_1_BIT, params.usage | VK_IMAGE_USAGE_SAMPLED_BIT, false);
//	ASSERT(mImage->isValid(), "[Graphics::StorageImage] Failed to create image!");
//	mMemoryObject = new VulkanMemoryObject();
//	ASSERT(mMemoryObject->createImageMemoryObject(physicalDevice, device, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, mImage->getImage()), "[Graphics::Image2D] Failed to allocate and bind memory object");
//	mImageView = new VulkanImageView(device, mImage->getImage(), params.viewType, params.format, params.aspect);
//}
//
//bool StorageImage::destroy(VkDevice device)
//{
//	return mImageView->destroy(device) && mMemoryObject->destroy(device) && mImage->destroy(device);
//}
//
//StorageImage::~StorageImage()
//{
//	SafeDelete(mImageView);
//	SafeDelete(mImage);
//	SafeDelete(mMemoryObject);
//}
