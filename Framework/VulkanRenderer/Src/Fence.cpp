#include "Precompiled.h"
#include "Fence.h"
#include "VulkanFunctions.h"
using namespace SingularityEngine::Vulkan;

bool Fence::create(VkDevice device, bool signaled)
{
	VkFenceCreateInfo fenceCreateInfo = {
		VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
		nullptr,
		signaled ? VK_FENCE_CREATE_SIGNALED_BIT : static_cast<VkFenceCreateFlags>(0)
	};

	if (vkCreateFence(device, &fenceCreateInfo, nullptr, &mFence) != VK_SUCCESS)
	{
		LOG("[Graphics::Fence] Failed creation!");
		return false;
	}
	mInitialized = true;
	return true;
}

bool Fence::destroy(VkDevice device)
{
	if (mFence != VK_NULL_HANDLE)
	{
		vkDestroyFence(device, mFence, nullptr);
		mFence = VK_NULL_HANDLE;
	}
	return true;
}
