#pragma once
#include "Common.h"

namespace SingularityEngine::Graphics
{
	class VulkanImageView
	{
	public:
		VulkanImageView(VkImage image, VkImageViewType viewType, VkFormat format, VkImageAspectFlags aspect);
		~VulkanImageView() { destroy(); }
		operator VkImageView() { return mImageView; }


	private:
		bool destroy();
		VkImageView mImageView;
	};
}