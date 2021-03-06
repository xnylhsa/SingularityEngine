#include "Precompiled.h"
#include "VulkanRenderPass.h"
#include "VulkanFunctions.h"
#include "VulkanContext.h"
using namespace SingularityEngine::Graphics;

VulkanRenderPass::VulkanRenderPass(std::vector<VkAttachmentDescription>& attachmentDescriptions, std::vector<VkSubpassDescription>& subpassParameters, std::vector<VkSubpassDependency>& subpassDependencies)
{
	std::shared_ptr<VulkanDevice> device = VulkanContext::GetVulkanDevice();

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
	if (vkCreateRenderPass(*device, &renderPassCreateInfo, nullptr, &mRenderPass) != VK_SUCCESS)
	{
		ASSERT(false, "[Graphics::RenderPass] Failed creation!");
	}
}

VulkanRenderPass::~VulkanRenderPass()
{
	if (VK_NULL_HANDLE != mRenderPass) {
		std::shared_ptr<VulkanDevice> device = VulkanContext::GetVulkanDevice();
		vkDestroyRenderPass(*device, mRenderPass, nullptr);
		mRenderPass = VK_NULL_HANDLE;
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

