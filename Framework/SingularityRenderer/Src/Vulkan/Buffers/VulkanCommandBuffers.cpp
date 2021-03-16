#include "Precompiled.h"
#include "Vulkan/Buffers/VulkanCommandBuffers.h"
#include "Vulkan/Util/VulkanFunctions.h"
#include "Vulkan/Device/VulkanDevice.h"
using namespace SingularityEngine::SERenderer;

VulkanCommandBuffers::VulkanCommandBuffers()
{

}

bool VulkanCommandBuffers::Create(std::weak_ptr<VulkanDevice> device, VulkanCommandPool& cmdPool, VkCommandBufferLevel level, uint32_t count)
{
	mDevice = device;
	mBufferCount = count;
	for (size_t i = 0; i < mBufferCount; i++)
	{
		if(level == VK_COMMAND_BUFFER_LEVEL_PRIMARY)
			mCommandBuffers.push_back(cmdPool.requestCommandBuffer());
		else if (level == VK_COMMAND_BUFFER_LEVEL_SECONDARY)
			mCommandBuffers.push_back(cmdPool.requestSecondaryCommandBuffer());
	}
	return true;

}

bool VulkanCommandBuffers::beginRecording(uint32_t index, VkCommandBufferUsageFlags usage, VkCommandBufferInheritanceInfo* secondaryCommandBufferInfo)
{
	VkCommandBufferBeginInfo commandBufferBeginInfo = {
		VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
		nullptr,
		usage,
		secondaryCommandBufferInfo
	};
	
	if (vkBeginCommandBuffer(mCommandBuffers[index], &commandBufferBeginInfo) != VK_SUCCESS)
	{
		LOG("[Graphics::CommandBuffer] Failed to begin recording!");
		return false;
	}
	return true;
}

bool VulkanCommandBuffers::stopRecording(uint32_t index)
{
	if (vkEndCommandBuffer(mCommandBuffers[index]) != VK_SUCCESS)
	{
		LOG("[Graphics::CommandBuffer] Failed to end recording command buffer, error occured during command buffer recording");
		return false;
	}
	return true;
}

bool VulkanCommandBuffers::resetBuffer(uint32_t index, bool releaseResources)
{
	if (vkResetCommandBuffer(mCommandBuffers[index], releaseResources ? VK_COMMAND_BUFFER_RESET_RELEASE_RESOURCES_BIT : 0) != VK_SUCCESS)
	{
		LOG("[Graphics::CommandBuffer] Error occured during reset!");
		return false;
	}
	return true;
}
