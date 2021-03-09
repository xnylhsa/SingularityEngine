#include "Precompiled.h"
#include "Vulkan/SwapChain/VulkanDepthStencil.h"
#include "Vulkan/Texture/VulkanImage.h"
#include "Vulkan/Texture/VulkanImageView.h"
#include "Vulkan/Memory/VulkanMemoryObject.h"
#include "Vulkan/Device/VulkanDevice.h"
#include "vulkan/SwapChain/VulkanSwapChain.h"
#include "Vulkan/Memory/VulkanMemoryAllocator.h"
using namespace SingularityEngine::SERenderer;



VulkanDepthStencil::VulkanDepthStencil(std::weak_ptr<VulkanDevice> device, VulkanSwapChain* swapChain, VulkanMemoryAllocator* allocator)
{

	auto vulkanDevice = device.lock();
	ASSERT(vulkanDevice, "[SERenderer::VulkanImage] device lost.");
	std::vector<VkFormat> depthFormats = {
	VK_FORMAT_D32_SFLOAT_S8_UINT,
	VK_FORMAT_D32_SFLOAT,
	VK_FORMAT_D24_UNORM_S8_UINT,
	VK_FORMAT_D16_UNORM_S8_UINT,
	VK_FORMAT_D16_UNORM
	};
	mDepthStencilFormat = vulkanDevice->findFirstCompatibleDepthFormat(depthFormats);
	if (mDepthStencilFormat == VK_FORMAT_MAX_ENUM)
	{
		return;
	}
	VkExtent2D surfaceExtent = swapChain->getSurfaceExtent();
	VkExtent3D extent = { surfaceExtent.width, surfaceExtent.height, 1 };
	mImage = std::make_unique<VulkanImage>(device, VK_IMAGE_TYPE_2D,
		mDepthStencilFormat,
		extent,
		1,
		1,
		VK_SAMPLE_COUNT_1_BIT,
		VK_IMAGE_TILING_OPTIMAL,
		VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT,
		false);
	ASSERT(mImage->isValid(), "[Graphics::Image2D] Failed to create image!");
	mMemoryObject = allocator->allocateImageMemory(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, *mImage);
	ASSERT(mMemoryObject, "[Graphics::Image2D] Failed to allocate and bind memory object");
	mImageView = std::make_unique<VulkanImageView>(device, *mImage, VK_IMAGE_VIEW_TYPE_2D, mDepthStencilFormat, mDepthStencilFormat >= VK_FORMAT_D16_UNORM_S8_UINT ? VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT : VK_IMAGE_ASPECT_DEPTH_BIT);
}

VulkanDepthStencil::~VulkanDepthStencil()
{
	mImageView.reset();
	mMemoryObject.reset();
	mImage.reset();
	mImageView = nullptr;
	mImage = nullptr;
	mMemoryObject = nullptr;
}

VulkanImageView* VulkanDepthStencil::getImageView()
{
	return mImageView.get(); 
}

VulkanImage* VulkanDepthStencil::getImage()
{
	return mImage.get();
}

VulkanMemoryObject* VulkanDepthStencil::getMemoryObject()
{
	return mMemoryObject.get();
}
