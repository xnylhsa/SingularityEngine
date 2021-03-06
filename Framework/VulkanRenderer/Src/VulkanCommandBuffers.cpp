#include "Precompiled.h"
#include "VulkanCommandBuffers.h"
#include "VulkanFunctions.h"
#include "VulkanContext.h"
using namespace SingularityEngine::Graphics;

VulkanCommandBuffers::VulkanCommandBuffers()
{

}

bool VulkanCommandBuffers::Create(VulkanCommandPool& cmdPool, VkCommandBufferLevel level, uint32_t count)
{
	std::shared_ptr<VulkanDevice> vulkanDevice = VulkanContext::GetVulkanDevice();
	mBufferCount = count;
	VkCommandBufferAllocateInfo command_buffer_allocate_info = {
	  VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
	  nullptr,
	  cmdPool,
	  level,
	  mBufferCount
	};

	mCommandBuffers.resize(mBufferCount);
	if (vkAllocateCommandBuffers(*vulkanDevice, &command_buffer_allocate_info, mCommandBuffers.data()) != VK_SUCCESS)
	{
		LOG("");
		return false;
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

bool VulkanCommandBuffers::Destroy(VulkanCommandPool& cmdPool)
{
	std::shared_ptr<VulkanDevice> vulkanDevice = VulkanContext::GetVulkanDevice();

	if (mCommandBuffers.size() > 0)
	{
		vkFreeCommandBuffers(*vulkanDevice, cmdPool, static_cast<uint32_t>(mCommandBuffers.size()), &mCommandBuffers[0]);
		mCommandBuffers.clear();
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
		LOG("[Graphics::CommandBuffer] Error occurder during reset!");
		return false;
	}
	return true;
}
