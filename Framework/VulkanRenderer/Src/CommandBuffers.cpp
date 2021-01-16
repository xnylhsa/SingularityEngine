#include "Precompiled.h"
#include "CommandBuffers.h"
#include "VulkanFunctions.h"
using namespace SingularityEngine::Vulkan;

CommandBuffers::CommandBuffers()
{

}

bool CommandBuffers::Create(VkDevice& device, VkCommandPool& cmdPool, VkCommandBufferLevel level, uint32_t count)
{
	mBufferCount = count;
	VkCommandBufferAllocateInfo command_buffer_allocate_info = {
	  VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
	  nullptr,
	  cmdPool,
	  level,
	  mBufferCount
	};

	mCommandBuffers.resize(mBufferCount);
	if (vkAllocateCommandBuffers(device, &command_buffer_allocate_info, mCommandBuffers.data()) != VK_SUCCESS)
	{
		LOG("");
		return false;
	}

	return true;

}

bool CommandBuffers::beginRecording(uint32_t index, VkCommandBufferUsageFlags usage, VkCommandBufferInheritanceInfo* secondaryCommandBufferInfo)
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

bool CommandBuffers::Destroy(VkDevice& device, VkCommandPool& cmdPool)
{
	if (mCommandBuffers.size() > 0)
	{
		vkFreeCommandBuffers(device, cmdPool, static_cast<uint32_t>(mCommandBuffers.size()), &mCommandBuffers[0]);
		mCommandBuffers.clear();
	}
	return true;
}

bool CommandBuffers::stopRecording(uint32_t index)
{
	if (vkEndCommandBuffer(mCommandBuffers[index]) != VK_SUCCESS)
	{
		LOG("[Graphics::CommandBuffer] Failed to end recording command buffer, error occured during command buffer recording");
		return false;
	}
	return true;
}

bool CommandBuffers::resetBuffer(uint32_t index, bool releaseResources)
{
	if (vkResetCommandBuffer(mCommandBuffers[index], releaseResources ? VK_COMMAND_BUFFER_RESET_RELEASE_RESOURCES_BIT : 0) != VK_SUCCESS)
	{
		LOG("[Graphics::CommandBuffer] Error occurder during reset!");
		return false;
	}
	return true;
}
