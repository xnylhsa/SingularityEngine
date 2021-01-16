#pragma once
#include "Common.h"

namespace SingularityEngine::Vulkan
{
	class ImageView
	{
	public:
		ImageView(VkDevice& device, VkImage& image, VkImageViewType viewType, VkFormat format, VkImageAspectFlags aspect);
		bool destroy(VkDevice& device);
		~ImageView();


	private:
		VkImageView mImageView;
	};
}