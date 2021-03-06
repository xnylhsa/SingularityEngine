#pragma once
#include "Common.h"

namespace SingularityEngine::Graphics
{
	class VulkanFrameBuffer
	{
	public:
		VulkanFrameBuffer() = default;
		operator VkFramebuffer() { return mFrameBuffer; }
		void create(VkRenderPass renderPass, std::vector<VkImageView> attachments, uint32_t width, uint32_t height, uint32_t layers);
		void destroy();
		~VulkanFrameBuffer() = default;
	private:
		VkFramebuffer mFrameBuffer = VK_NULL_HANDLE;
	};
}