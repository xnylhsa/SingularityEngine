#pragma once
#include "Common.h"

namespace SingularityEngine::SERenderer
{
	class VulkanDevice;
	class VulkanFrameBuffer
	{
	public:
		VulkanFrameBuffer() = default;
		operator VkFramebuffer() { return mFrameBuffer; }
		void create(std::weak_ptr<VulkanDevice> device, VkRenderPass renderPass, std::vector<VkImageView> attachments, uint32_t width, uint32_t height, uint32_t layers);
		void destroy();
		~VulkanFrameBuffer() = default;
	private:
		VkFramebuffer mFrameBuffer = VK_NULL_HANDLE;
		std::weak_ptr<VulkanDevice> mDevice;
	};
}