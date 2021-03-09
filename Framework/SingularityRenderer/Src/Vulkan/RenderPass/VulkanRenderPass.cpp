#include "Precompiled.h"
#include "Vulkan/RenderPass/VulkanRenderPass.h"
#include "Vulkan/Util/VulkanFunctions.h"
#include "Vulkan/Context/VulkanContext.h"
using namespace SingularityEngine::SERenderer;

VulkanRenderPass::VulkanRenderPass(std::weak_ptr<VulkanDevice> device, std::vector<VkAttachmentDescription>& attachmentDescriptions, std::vector<VkSubpassDescription>& subpassParameters, std::vector<VkSubpassDependency>& subpassDependencies): mDevice(device)
{
	auto vulkanDevice = mDevice.lock();
	ASSERT(vulkanDevice, "[SERenderer::VulkanRenderPass] device lost.");
	VkRenderPassCreateInfo renderPassCreateInfo = {
		VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
		nullptr,
		0,
		static_cast<uint32_t>(attachmentDescriptions.size()),
		attachmentDescriptions.data(),
		static_cast<uint32_t>(subpassParameters.size()),
		subpassParameters.data(),
		static_cast<uint32_t>(subpassDependencies.size()),
		subpassDependencies.data()
	};
	if (vkCreateRenderPass(*vulkanDevice, &renderPassCreateInfo, nullptr, &mRenderPass) != VK_SUCCESS)
	{
		ASSERT(false, "[SERenderer::VulkanRenderPass] Failed creation!");
	}
}

VulkanRenderPass::~VulkanRenderPass()
{
	auto vulkanDevice = mDevice.lock();
	ASSERT(vulkanDevice, "[SERenderer::VulkanRenderPass] device lost.");
	if (vulkanDevice && VK_NULL_HANDLE != mRenderPass) {
		vkDestroyRenderPass(*vulkanDevice, mRenderPass, nullptr);
		mRenderPass = VK_NULL_HANDLE;
		mDevice.reset();
	}
}

void VulkanRenderPass::beginRendering(VkCommandBuffer commandBuffer, VkFramebuffer framebuffer, VkRect2D renderArea, std::vector<VkClearValue> clearValues, VkSubpassContents subpassContents)
{
	VkRenderPassBeginInfo renderPassBeginInfo = {
		  VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
		  nullptr,
		  mRenderPass,
		  framebuffer,
		  renderArea,
		  static_cast<uint32_t>(clearValues.size()),
		  clearValues.data()
	};
	vkCmdBeginRenderPass(commandBuffer, &renderPassBeginInfo, subpassContents);
}

void VulkanRenderPass::transitionToNextSubpass(VkCommandBuffer commandBuffer, VkSubpassContents subpassContents)
{
	vkCmdNextSubpass(commandBuffer, subpassContents);
}

void VulkanRenderPass::endRendering(VkCommandBuffer commandBuffer)
{
	vkCmdEndRenderPass(commandBuffer);
}

