#pragma once
#include "Common.h"
namespace SingularityEngine::SERenderer
{
	class VulkanMemoryObject;
	class VulkanImage;
	class VulkanImageView;
	class VulkanMemoryAllocator;
	class VulkanDevice;
	class VulkanSwapChain;
	class VulkanDepthStencil
	{
	public:
		VulkanDepthStencil(std::weak_ptr<VulkanDevice> device, VulkanSwapChain* swapChain, VulkanMemoryAllocator* allocator);
		~VulkanDepthStencil();
		VulkanImageView* getImageView();
		VulkanImage* getImage();
		VulkanMemoryObject* getMemoryObject();
		VkFormat getDepthStencilFormat() { return mDepthStencilFormat; }
	private:
		std::unique_ptr<VulkanMemoryObject> mMemoryObject = nullptr;
		std::unique_ptr<VulkanImage> mImage = nullptr;
		std::unique_ptr<VulkanImageView> mImageView = nullptr;
		VkFormat mDepthStencilFormat = VK_FORMAT_MAX_ENUM;
	};
}
