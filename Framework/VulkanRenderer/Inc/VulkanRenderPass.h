#pragma once
#include "Common.h"
#include "SubpassParameters.h"


namespace SingularityEngine::Graphics
{
	class VulkanRenderPass
	{
	public:
		VulkanRenderPass(std::vector<VkAttachmentDescription>& attachmentDescriptions, std::vector<VkSubpassDescription>& subpassParameters, std::vector<VkSubpassDependency>& subpassDependencies);
		~VulkanRenderPass();
		void beginRendering(VkCommandBuffer commandBuffer, VkFramebuffer framebuffer, VkRect2D renderArea, std::vector<VkClearValue> clearValues, VkSubpassContents subpassContents);
		void transitionToNextSubpass(VkCommandBuffer commandBuffer, VkSubpassContents subpassContents);
		void endRendering(VkCommandBuffer commandBuffer);
		operator VkRenderPass() { return mRenderPass; }
	private:
		VkRenderPass mRenderPass = VK_NULL_HANDLE;
		//std::vector<Math::Color> mClearValues;
		//std::unique_ptr<VulkanSemaphore> mWaitSemaphore;
		//std::vector<VulkanFrameBuffer> mFrameBuffers;
		//VulkanCommandBuffers mCommandBuffers;
		//VulkanCommandPool mCommandPool;
	};
}