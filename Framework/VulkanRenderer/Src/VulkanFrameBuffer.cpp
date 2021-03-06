#include "Precompiled.h"
#include "VulkanFrameBuffer.h"
#include "VulkanFunctions.h"
#include "VulkanContext.h"
using namespace SingularityEngine::Graphics;
void VulkanFrameBuffer::create(VkRenderPass renderPass, std::vector<VkImageView> attachments, uint32_t width, uint32_t height, uint32_t layers)
{
	std::shared_ptr<VulkanDevice> device = VulkanContext::GetVulkanDevice();
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
	VkResult result = vkCreateFramebuffer(*device, &framebufferCreateInfo, nullptr, &mFrameBuffer);
	if (VkResult::VK_SUCCESS != result) {
		ASSERT(false, "Could not create a framebuffer.");
	}
}

void VulkanFrameBuffer::destroy()
{
	if (VK_NULL_HANDLE != mFrameBuffer) {
		std::shared_ptr<VulkanDevice> device = VulkanContext::GetVulkanDevice();

		vkDestroyFramebuffer(*device, mFrameBuffer, nullptr);
		mFrameBuffer = VK_NULL_HANDLE;
	}
}
