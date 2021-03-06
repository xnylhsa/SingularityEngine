#include "Precompiled.h"
//#include "Image2D.h"
//using namespace SingularityEngine::Graphics;
//
//
//
//Image2D::Image2D(VkPhysicalDevice physicalDevice, VkDevice device, VkFormat format, VkExtent2D size, uint32_t numMipMaps, uint32_t numLayers, VkSampleCountFlagBits samples, VkImageUsageFlags usage, VkImageAspectFlags aspect)
//{
//	mImage = new VulkanImage(device, VK_IMAGE_TYPE_2D, format, {size.width, size.height, 1}, numMipMaps, numLayers, samples, usage, false);
//	ASSERT(mImage->isValid(), "[Graphics::Image2D] Failed to create image!");
//	mMemoryObject = new VulkanMemoryObject();
//	ASSERT(mMemoryObject->createImageMemoryObject(physicalDevice, device, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, mImage->getImage()), "[Graphics::Image2D] Failed to allocate and bind memory object");
//	mImageView = new VulkanImageView(device, mImage->getImage(), VK_IMAGE_VIEW_TYPE_2D, format, aspect);
//}
//
//Image2D::~Image2D()
//{
//	SafeDelete(mImageView);
//	SafeDelete(mImage);
//	SafeDelete(mMemoryObject);
//}
//
//bool Image2D::destroy(VkDevice device)
//{
//	return mImageView->destroy(device) && mMemoryObject->destroy(device) && mImage->destroy(device);
//}
