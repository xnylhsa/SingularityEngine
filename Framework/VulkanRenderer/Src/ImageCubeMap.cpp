#include "Precompiled.h"
//#include "ImageCubeMap.h"
//
//using namespace SingularityEngine::Graphics;
//
//ImageCubeMap::ImageCubeMap(VkPhysicalDevice physicalDevice, VkDevice device, VkExtent2D size, uint32_t numMipMaps, VkImageUsageFlags usage, VkImageAspectFlags aspect)
//{
//	mImage = new VulkanImage(device, VK_IMAGE_TYPE_2D, VK_FORMAT_R8G8B8A8_UNORM, { size.width, size.height, 1 }, numMipMaps, 6, VK_SAMPLE_COUNT_1_BIT, usage, true);
//	ASSERT(mImage->isValid(), "[Graphics::ImageCubeMap] Failed to create image!");
//	mMemoryObject = new VulkanMemoryObject();
//	ASSERT(mMemoryObject->createImageMemoryObject(physicalDevice, device, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, mImage->getImage()), "[Graphics::Image2D] Failed to allocate and bind memory object");
//	mImageView = new VulkanImageView(device, mImage->getImage(), VK_IMAGE_VIEW_TYPE_2D, VK_FORMAT_R8G8B8A8_UNORM, aspect);
//}
//
//bool ImageCubeMap::destroy(VkDevice device)
//{
//	return mImageView->destroy(device) && mMemoryObject->destroy(device) && mImage->destroy(device);
//}
//
//ImageCubeMap::~ImageCubeMap()
//{
//	SafeDelete(mImageView);
//	SafeDelete(mImage);
//	SafeDelete(mMemoryObject);
//}
