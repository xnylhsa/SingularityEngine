#include "Precompiled.h"
#include "Vulkan/Texture/VulkanImageView.h"
#include "Vulkan/Util/VulkanFunctions.h"
#include "Vulkan/Context/VulkanContext.h"
using namespace SingularityEngine::SERenderer;

VulkanImageView::VulkanImageView(std::weak_ptr<VulkanDevice> device, VkImage image, VkImageViewType viewType, VkFormat format, VkImageAspectFlags aspect): mDevice(device)
{
	ASSERT(!mDevice.expired(), "[SERenderer::VulkanImageView] device is not valid.");
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

	if (auto vulkanDevice = mDevice.lock())
	{
		ASSERT(VK_SUCCESS == vkCreateImageView(*vulkanDevice, &viewImageCreateInfo, nullptr, &mImageView), "[Graphics::ImageView] Failed Creation!");
	}

}

bool VulkanImageView::destroy()
{
	auto vulkanDevice = mDevice.lock();
	if (mImageView != VK_NULL_HANDLE && vulkanDevice != nullptr)
	{
		vkDestroyImageView(*vulkanDevice, mImageView, nullptr);
		mImageView = VK_NULL_HANDLE;
	}
	return true;
}
