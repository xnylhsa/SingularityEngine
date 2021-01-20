#include "Precompiled.h"
#include "FrameBuffer.h"
#include "VulkanFunctions.h"

using namespace SingularityEngine::Vulkan;

bool FrameBuffer::create(VkDevice device, VkRenderPass renderPass, std::vector<VkImageView> attachments, uint32_t width, uint32_t height, uint32_t layers)
{
	VkFramebufferCreateInfo framebufferCreateInfo = {
		VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
		nullptr,
		0,
		renderPass,
		static_cast<uint32_t>(attachments.size()),
		attachments.data(),
		width,
		height,
		layers
	};
	VkResult result = vkCreateFramebuffer(device, &framebufferCreateInfo, nullptr, &mFrameBuffer);
	if (VK_SUCCESS != result) {
		std::cout << "Could not create a framebuffer." << std::endl;
		return false;
	}
	return true;
}

bool FrameBuffer::destroy(VkDevice device)
{
	if (VK_NULL_HANDLE != mFrameBuffer) {
		vkDestroyFramebuffer(device, mFrameBuffer, nullptr);
		mFrameBuffer = VK_NULL_HANDLE;
	}
	return true;
}
