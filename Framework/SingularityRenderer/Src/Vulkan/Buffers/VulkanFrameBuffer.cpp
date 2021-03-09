#include "Precompiled.h"
#include "Vulkan/Buffers/VulkanFrameBuffer.h"
#include "Vulkan/Util/VulkanFunctions.h"
#include "Vulkan/Context/VulkanContext.h"
using namespace SingularityEngine::SERenderer;
void VulkanFrameBuffer::create(std::weak_ptr<VulkanDevice> device, VkRenderPass renderPass, std::vector<VkImageView> attachments, uint32_t width, uint32_t height, uint32_t layers)
{
	mDevice = device;
	auto vulkanDevice = mDevice.lock();
	ASSERT(vulkanDevice, "[SERenderer::VulkanImage] device lost.");
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
	VkResult result = vkCreateFramebuffer(*vulkanDevice, &framebufferCreateInfo, nullptr, &mFrameBuffer);
	if (VkResult::VK_SUCCESS != result) {
		ASSERT(false, "Could not create a framebuffer.");
	}
}

void VulkanFrameBuffer::destroy()
{
	auto vulkanDevice = mDevice.lock();
	ASSERT(vulkanDevice, "[SERenderer::VulkanImage] device lost.");
	if (vulkanDevice && VK_NULL_HANDLE != mFrameBuffer) {
		vkDestroyFramebuffer(*vulkanDevice, mFrameBuffer, nullptr);
		mFrameBuffer = VK_NULL_HANDLE;
		mDevice.reset();
	}
}
