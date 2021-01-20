#pragma once
#include "Common.h"
#include "SubpassParameters.h"


namespace SingularityEngine::Vulkan
{
	class RenderPass
	{
	public:
		bool create(VkDevice device, std::vector<VkAttachmentDescription> const & attachmentDescriptions, std::vector<SubpassParameters> const& subpassParameters, std::vector<VkSubpassDependency> const& subpassDependencies);
		bool destroy(VkDevice device);
		bool beginRendering(VkCommandBuffer commandBuffer, VkFramebuffer framebuffer, VkRect2D renderArea, std::vector<VkClearValue> clearValues, VkSubpassContents subpassContents);
		bool transitionToNextSubpass(VkCommandBuffer commandBuffer, VkSubpassContents subpassContents);
		bool endRendering(VkCommandBuffer commandBuffer);
	private:
		VkRenderPass mRenderPass = VK_NULL_HANDLE;
	};
}