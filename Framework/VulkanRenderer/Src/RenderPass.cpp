#include "Precompiled.h"
#include "RenderPass.h"
#include "VulkanFunctions.h"

using namespace SingularityEngine::Vulkan;

bool RenderPass::create(VkDevice device, std::vector<VkAttachmentDescription> const& attachmentDescriptions, std::vector<SubpassParameters> const& subpassParameters, std::vector<VkSubpassDependency> const& subpassDependencies)
{
	std::vector<VkSubpassDescription> subpassDescriptions = ConvertSubpassDescriptions(subpassParameters);

	VkRenderPassCreateInfo renderPassCreateInfo = {
		VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
		nullptr,
		0,
		static_cast<uint32_t>(attachmentDescriptions.size()),
		attachmentDescriptions.data(),
		static_cast<uint32_t>(subpassDescriptions.size()),
		subpassDescriptions.data(),
		static_cast<uint32_t>(subpassDependencies.size()),
		subpassDependencies.data()
	};
	if (vkCreateRenderPass(device, &renderPassCreateInfo, nullptr, &mRenderPass) != VK_SUCCESS)
	{
		LOG("[Graphics::RenderPass] Failed creation!");
		return false;
	}
	return true;
}

bool RenderPass::destroy(VkDevice device)
{
	if (VK_NULL_HANDLE != mRenderPass) {
		vkDestroyRenderPass(device, mRenderPass, nullptr);
		mRenderPass = VK_NULL_HANDLE;
	}
	return true;
}

bool RenderPass::beginRendering(VkCommandBuffer commandBuffer, VkFramebuffer framebuffer, VkRect2D renderArea, std::vector<VkClearValue> clearValues, VkSubpassContents subpassContents)
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

bool RenderPass::transitionToNextSubpass(VkCommandBuffer commandBuffer, VkSubpassContents subpassContents)
{
	vkCmdNextSubpass(commandBuffer, subpassContents);
}

bool RenderPass::endRendering(VkCommandBuffer commandBuffer)
{
	vkCmdEndRenderPass(commandBuffer);
}

