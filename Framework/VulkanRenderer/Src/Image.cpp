#include "Precompiled.h"
#include "Image.h"
#include "VulkanFunctions.h"

using namespace SingularityEngine::Vulkan;

Image::Image(VkDevice device, VkImageType type, VkFormat format, VkExtent3D size, uint32_t numMipmaps, uint32_t layers, VkSampleCountFlagBits samples, VkImageUsageFlags usageScenarios, bool isCubemap) : mImageType(type), mFormat(format), mExtent(size), mNumberOfMipmaps(numMipmaps), mNumberOfLayers(layers), mSamples(samples), mUsageFlags(usageScenarios), mIsCubemap(isCubemap)
{

	VkImageCreateInfo creationInfo = createInfo();

	ASSERT(vkCreateImage(device, &creationInfo, nullptr, &mImage) == VK_SUCCESS, "[Grahpics::Image] Failed to create image!");

}

Image::~Image()
{
	ASSERT(mImage == VK_NULL_HANDLE, "[Graphics::Image] Failed to destroy image before destructor was called!");
}

VkImageCreateInfo Image::createInfo()
{
	VkImageCreateInfo imageCreationInfo = {
  VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
  nullptr,
  mIsCubemap ? VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT : 0u,
  mImageType,
  mFormat,
  mExtent,
  mNumberOfMipmaps,
  mIsCubemap ? 6 * mNumberOfLayers : mNumberOfLayers,
  mSamples,
  VK_IMAGE_TILING_OPTIMAL,
  mUsageFlags,
  VK_SHARING_MODE_EXCLUSIVE,
  0,
  nullptr,
  VK_IMAGE_LAYOUT_UNDEFINED
	};
	return imageCreationInfo;
}

bool Image::destroy(VkDevice device)
{
	if (mImage != VK_NULL_HANDLE)
	{
		vkDestroyImage(device, mImage, nullptr);
		mImage = VK_NULL_HANDLE;
	}
	return true;
}

void Image::copyDataFromBuffer(VkCommandBuffer commandBuffer, VkBuffer srcBuffer, VkImageLayout layout, std::vector<VkBufferImageCopy> regions)
{
	if (regions.size() > 0)
	{
		vkCmdCopyBufferToImage(commandBuffer, srcBuffer, mImage, layout, static_cast<uint32_t>(regions.size()), regions.data());
	}
}

void Image::copyDataToBuffer(VkCommandBuffer commandBuffer, VkBuffer dstBuffer, VkImageLayout layout, std::vector<VkBufferImageCopy> regions)
{
	if (regions.size() > 0)
	{
		vkCmdCopyImageToBuffer(commandBuffer, mImage, layout, dstBuffer, static_cast<uint32_t>(regions.size()), regions.data());
	}
}
