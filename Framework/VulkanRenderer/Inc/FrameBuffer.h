#pragma once
#include "Common.h"

namespace SingularityEngine::Vulkan
{
	class FrameBuffer
	{
	public:
		bool create(VkDevice device, VkRenderPass renderPass, std::vector<VkImageView> attachments, uint32_t width, uint32_t height, uint32_t layers);
		bool destroy(VkDevice device);
	private:
		VkFramebuffer mFrameBuffer = VK_NULL_HANDLE;
	};
}