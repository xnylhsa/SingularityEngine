#include "Precompiled.h"
#include "VulkanFence.h"
#include "VulkanFunctions.h"
#include "VulkanContext.h"
using namespace SingularityEngine::Graphics;

void VulkanFence::create(bool signaled)
{
	std::shared_ptr<VulkanDevice> device = VulkanContext::GetVulkanDevice();

	VkFenceCreateInfo fenceCreateInfo = {
		VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
		nullptr,
		signaled ? VK_FENCE_CREATE_SIGNALED_BIT : static_cast<VkFenceCreateFlags>(0)
	};

	if (vkCreateFence(*device, &fenceCreateInfo, nullptr, &mFence) != VK_SUCCESS)
	{
		LOG("[Graphics::Fence] Failed creation!");
	}
}

void VulkanFence::destroy()
{
	if (mFence != VK_NULL_HANDLE)
	{
		std::shared_ptr<VulkanDevice> device = VulkanContext::GetVulkanDevice();
		vkDestroyFence(*device, mFence, nullptr);
		mFence = VK_NULL_HANDLE;
	}
}

bool VulkanFence::waitForFences(std::vector<VkFence> fences, bool waitForAll, uint64_t timeout)
{
	std::shared_ptr<VulkanDevice> device = VulkanContext::GetVulkanDevice();
	if (!fences.empty() && vkWaitForFences(*device, static_cast<uint32_t>(fences.size()), &fences[0], static_cast<VkBool32>(waitForAll), timeout) != VK_SUCCESS)
	{
		LOG("[Graphics::fences] Waiting on fences failed!");
		return false;
	}
	return true;
}

bool VulkanFence::resetFences(std::vector<VkFence> fences)
{
	std::shared_ptr<VulkanDevice> device = VulkanContext::GetVulkanDevice();

	if (!fences.empty() && vkResetFences(*device, static_cast<uint32_t>(fences.size()), &fences[0]) != VK_SUCCESS)
	{
		LOG("[Graphics::Fences] Failed to reset fences!");
		return false;
	}
	return true;
}
