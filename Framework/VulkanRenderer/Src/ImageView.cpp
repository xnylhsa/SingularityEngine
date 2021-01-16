#include "Precompiled.h"
#include "ImageView.h"
#include "VulkanFunctions.h"

using namespace SingularityEngine::Vulkan;

ImageView::ImageView(VkDevice& device, VkImage& image, VkImageViewType viewType, VkFormat format, VkImageAspectFlags aspect)
{
	VkImageViewCreateInfo viewImageCreateInfo = {
		VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
		nullptr,
		0,
		image,
		viewType,
		format,
		{
			VK_COMPONENT_SWIZZLE_IDENTITY,
			VK_COMPONENT_SWIZZLE_IDENTITY,
			VK_COMPONENT_SWIZZLE_IDENTITY,
			VK_COMPONENT_SWIZZLE_IDENTITY
		},
		{
			aspect,
			0,
			VK_REMAINING_MIP_LEVELS,
			0,
			VK_REMAINING_ARRAY_LAYERS,
		}
	};

	ASSERT(VK_SUCCESS == vkCreateImageView(device, &viewImageCreateInfo, nullptr, &mImageView), "[Graphics::ImageView] Failed Creation!");

}

bool ImageView::destroy(VkDevice& device)
{
	if (mImageView != VK_NULL_HANDLE)
	{
		vkDestroyImageView(device, mImageView, nullptr);
		mImageView = VK_NULL_HANDLE;
	}
	return true;
}

ImageView::~ImageView()
{
	ASSERT(mImageView == VK_NULL_HANDLE, "[Graphics::ImageView] Image view was deleted before destroy() was called!");
}
