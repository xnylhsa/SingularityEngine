#include "Precompiled.h"
#include "VulkanCommandPool.h"
#include "VulkanFunctions.h"
#include "VulkanContext.h"
using namespace SingularityEngine::Graphics;


VulkanCommandPool::VulkanCommandPool()
{

}

bool VulkanCommandPool::create(uint32_t queueFamilyIndex, VkCommandBufferUsageFlags usageFlags)
{
	VkDevice device = *VulkanContext::GetVulkanDevice();
	mQueueFamilyIndex = queueFamilyIndex;
	VkCommandPoolCreateFlags parameters = usageFlags;
	VkCommandPoolCreateInfo cmdPoolCreateInfo = {
  VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
  nullptr,
  parameters,
  *mQueueFamilyIndex
	};

	if (vkCreateCommandPool(device, &cmdPoolCreateInfo, nullptr, &mCommandPool) != VK_SUCCESS)
	{
		LOG("[Graphics::CommandPool] Failed creation.");
		return false;
	}

	return true;
}

bool VulkanCommandPool::destroy()
{

	if (VK_NULL_HANDLE != mCommandPool) {
		VkDevice device = *VulkanContext::GetVulkanDevice();
		vkDestroyCommandPool(device, mCommandPool, nullptr);
		mCommandPool = VK_NULL_HANDLE;
	}
	return true;
}

bool VulkanCommandPool::reset(bool releaseResources)
{
	VkDevice device = *VulkanContext::GetVulkanDevice();
	if (vkResetCommandPool(device, mCommandPool, releaseResources ? VK_COMMAND_POOL_RESET_RELEASE_RESOURCES_BIT : 0) != VK_SUCCESS)
	{
		LOG("[Graphics::Comannd Pool] Error occured during reset!");
		return false;
	}
	return true;
}
