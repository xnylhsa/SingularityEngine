#pragma once
#include "Common.h"
namespace SingularityEngine::Graphics
{
	class VulkanMemoryObject;
	class VulkanImage;
	class VulkanImageView;
	class VulkanMemoryAllocator;
	class VulkanDepthStencil
	{
	public:
		VulkanDepthStencil(VulkanMemoryAllocator* allocator);
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
