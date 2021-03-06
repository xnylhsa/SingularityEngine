#include "Precompiled.h"
#include "VulkanImageView.h"
#include "VulkanFunctions.h"
#include "VulkanContext.h"
using namespace SingularityEngine::Graphics;

VulkanImageView::VulkanImageView(VkImage image, VkImageViewType viewType, VkFormat format, VkImageAspectFlags aspect)
{
	VkDevice device = *VulkanContext::GetVulkanDevice();
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

bool VulkanImageView::destroy()
{
	if (mImageView != VK_NULL_HANDLE)
	{
		VkDevice device = *VulkanContext::GetVulkanDevice();
		vkDestroyImageView(device, mImageView, nullptr);
		mImageView = VK_NULL_HANDLE;
	}
	return true;
}
