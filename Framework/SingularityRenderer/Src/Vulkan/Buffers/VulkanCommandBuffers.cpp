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
	auto vulkanDevice = mDevice.lock();
	ASSERT(vulkanDevice, "[SERenderer::VulkanImage] device lost.");
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
	auto vulkanDevice = mDevice.lock();
	ASSERT(vulkanDevice, "[SERenderer::VulkanImage] device lost.");
	if (vulkanDevice && !mCommandBuffers.empty())
	{
		vkFreeCommandBuffers(*vulkanDevice, cmdPool, static_cast<uint32_t>(mCommandBuffers.size()), &mCommandBuffers[0]);
		mCommandBuffers.clear();
		mDevice.reset();
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
