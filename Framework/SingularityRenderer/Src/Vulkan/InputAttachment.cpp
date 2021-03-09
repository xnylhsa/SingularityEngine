#include "Precompiled.h"
////#include "InputAttachment.h"
////#include "VulkanFunctions.h"
////
////using namespace SingularityEngine::Graphics;
////
////InputAttachment::InputAttachment(VkPhysicalDevice physicalDevice, VkDevice device, InputAttachment::InputAttachmentParams params)
////{
////	VkFormatProperties formatProperties;
////	vkGetPhysicalDeviceFormatProperties(physicalDevice, params.format, &formatProperties);
////	if (params.aspect & VK_IMAGE_ASPECT_COLOR_BIT && !(formatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT)) {
////		LOG("[Graphics::SampledImage] Provided format is not supported for a sampled image!");
////		return;
////	}
////
////	if (params.aspect & (VK_IMAGE_ASPECT_DEPTH_BIT) && !(formatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT)) {
////		LOG("[Graphics::SampledImage] Provided format is not supported for a sampled image!");
////		return;
////	}
////
////	mImage = new VulkanImage(device, params.type, params.format, params.size, 1, 1, VK_SAMPLE_COUNT_1_BIT, params.usage | VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT, false);
////	ASSERT(mImage->isValid(), "[Graphics::SampledImage] Failed to create image!");
////	mMemoryObject = new VulkanMemoryObject();
////	ASSERT(mMemoryObject->createImageMemoryObject(physicalDevice, device, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, mImage->getImage()), "[Graphics::Image2D] Failed to allocate and bind memory object");
////	mImageView = new VulkanImageView(device, mImage->getImage(), params.viewType, params.format, params.aspect);
////}
////
////bool InputAttachment::destroy(VkDevice device)
////{
////	return mImageView->destroy(device) && mMemoryObject->destroy(device) && mImage->destroy(device);
////}
////
////InputAttachment::~InputAttachment()
////{
////	SafeDelete(mImageView);
////	SafeDelete(mImage);
////	SafeDelete(mMemoryObject);
////}
