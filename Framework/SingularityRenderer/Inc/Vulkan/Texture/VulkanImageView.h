#pragma once
#include "Common.h"

namespace SingularityEngine::SERenderer
{
	class VulkanDevice;
	class VulkanImageView
	{
	public:
		VulkanImageView(std::weak_ptr<VulkanDevice> device, VkImage image, VkImageViewType viewType, VkFormat format, VkImageAspectFlags aspect);
		~VulkanImageView() { destroy(); }
		operator VkImageView() { return mImageView; }


	private:
		bool destroy();
		VkImageView mImageView;
		std::weak_ptr<VulkanDevice> mDevice;
	};
}