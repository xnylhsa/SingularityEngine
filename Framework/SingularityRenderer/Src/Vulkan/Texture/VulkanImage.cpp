#include "Precompiled.h"
#include "Vulkan/Texture/VulkanImage.h"
#include "Vulkan/Util/VulkanFunctions.h"
#include "Vulkan/Device/VulkanDevice.h"
using namespace SingularityEngine::SERenderer;

VulkanImage::VulkanImage(std::weak_ptr<VulkanDevice> device, VkImageType type, VkFormat format, VkExtent3D size, uint32_t numMipmaps, uint32_t layers, VkSampleCountFlagBits samples, VkImageUsageFlags usageScenarios, bool isCubemap) : mImageType(type), mFormat(format), mExtent(size), mNumberOfMipmaps(numMipmaps), mNumberOfLayers(layers), mSamples(samples), mUsageFlags(usageScenarios), mIsCubemap(isCubemap), mTiling(VK_IMAGE_TILING_OPTIMAL), mDevice(device)
{

	auto vulkanDevice = mDevice.lock();
	ASSERT(vulkanDevice, "[SERenderer::VulkanImage] device lost.");
	VkImageCreateInfo creationInfo = createInfo();
	ASSERT(vkCreateImage(*vulkanDevice, &creationInfo, nullptr, &mImage) == VK_SUCCESS, "[SERenderer::VulkanImage] Failed to create image!");
}

VulkanImage::VulkanImage(std::weak_ptr<VulkanDevice> device, VkImageType type, VkFormat format, VkExtent3D size, uint32_t numMipmaps, uint32_t layers, VkSampleCountFlagBits samples, VkImageTiling tiling, VkImageUsageFlags usageScenarios, bool isCubemap) : mImageType(type), mFormat(format), mExtent(size), mNumberOfMipmaps(numMipmaps), mNumberOfLayers(layers), mSamples(samples), mUsageFlags(usageScenarios), mIsCubemap(isCubemap), mTiling(tiling), mDevice(device)
{
	auto vulkanDevice = mDevice.lock();
	ASSERT(vulkanDevice, "[SERenderer::VulkanImage] device lost.");
	VkImageCreateInfo creationInfo = createInfo();
	ASSERT(vkCreateImage(*vulkanDevice, &creationInfo, nullptr, &mImage) == VK_SUCCESS, "[SERenderer::VulkanImage] Failed to create image!");
}

VulkanImage::~VulkanImage()
{
	destroy();
	ASSERT(mImage == VK_NULL_HANDLE, "[Graphics::VulkanImage] Failed to destroy image before destructor was called!");
}

VkImageCreateInfo VulkanImage::createInfo()
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
  mTiling,
  mUsageFlags,
  VK_SHARING_MODE_EXCLUSIVE,
  0,
  nullptr,
  VK_IMAGE_LAYOUT_UNDEFINED
	};
	return imageCreationInfo;
}

bool VulkanImage::destroy()
{
	auto vulkanDevice = mDevice.lock();
	if (vulkanDevice != nullptr && mImage != VK_NULL_HANDLE )
	{
		vkDestroyImage(*vulkanDevice, mImage, nullptr);
		mImage = VK_NULL_HANDLE;
	}
	return true;
}

void VulkanImage::copyDataFromBuffer(VkCommandBuffer commandBuffer, VkBuffer srcBuffer, VkImageLayout layout, std::vector<VkBufferImageCopy> regions)
{
	if (!regions.empty())
	{
		vkCmdCopyBufferToImage(commandBuffer, srcBuffer, mImage, layout, static_cast<uint32_t>(regions.size()), regions.data());
	}
}

void VulkanImage::copyDataToBuffer(VkCommandBuffer commandBuffer, VkBuffer dstBuffer, VkImageLayout layout, std::vector<VkBufferImageCopy> regions)
{
	if (!regions.empty())
	{
		vkCmdCopyImageToBuffer(commandBuffer, mImage, layout, dstBuffer, static_cast<uint32_t>(regions.size()), regions.data());
	}
}
