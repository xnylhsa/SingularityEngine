#include "Precompiled.h"
#include "CommandPool.h"
#include "VulkanFunctions.h"

using namespace SingularityEngine::Vulkan;


CommandPool::CommandPool()
{

}

bool CommandPool::create(VkDevice& device, uint32_t queueFamilyIndex)
{
	mQueueFamilyIndex = queueFamilyIndex;
	VkCommandPoolCreateFlags parameters = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
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

bool CommandPool::destroy(VkDevice& device)
{
	if (VK_NULL_HANDLE != mCommandPool) {
		vkDestroyCommandPool(device, mCommandPool, nullptr);
		mCommandPool = VK_NULL_HANDLE;
	}
	return true;
}

bool CommandPool::reset(VkDevice& device, bool releaseResources)
{
	if (vkResetCommandPool(device, mCommandPool, releaseResources ? VK_COMMAND_POOL_RESET_RELEASE_RESOURCES_BIT : 0) != VK_SUCCESS)
	{
		LOG("[Graphics::Comannd Pool] Error occured during reset!");
		return false;
	}
	return true;
}
