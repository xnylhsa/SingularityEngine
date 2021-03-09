#include "Precompiled.h"
#include "Vulkan/Synchronization/VulkanFence.h"
#include "Vulkan/Util/VulkanFunctions.h"
#include "Vulkan/Device/VulkanDevice.h"
using namespace SingularityEngine::SERenderer;

void VulkanFence::create(std::weak_ptr<VulkanDevice> device, bool signaled)
{
	mDevice = device;
	auto vulkanDevice = mDevice.lock();
	ASSERT(vulkanDevice, "[SERenderer::VulkanFence] device lost.");
	VkFenceCreateInfo fenceCreateInfo = {
		VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
		nullptr,
		signaled ? VK_FENCE_CREATE_SIGNALED_BIT : static_cast<VkFenceCreateFlags>(0)
	};

	if (vkCreateFence(*vulkanDevice, &fenceCreateInfo, nullptr, &mFence) != VK_SUCCESS)
	{
		LOG("[SERenderer::VulkanFence] Failed creation!");
	}
}

void VulkanFence::destroy()
{
	auto vulkanDevice = mDevice.lock();
	ASSERT(vulkanDevice, "[SERenderer::VulkanFence] device lost.");
	if (mFence != VK_NULL_HANDLE)
	{
		vkDestroyFence(*vulkanDevice, mFence, nullptr);
		mFence = VK_NULL_HANDLE;
		mDevice.reset();
		vulkanDevice = nullptr;
	}
}

bool VulkanFence::waitForFences(std::shared_ptr<VulkanDevice> device, std::vector<VkFence> fences, bool waitForAll, uint64_t timeout)
{
	if (!fences.empty() && vkWaitForFences(*device, static_cast<uint32_t>(fences.size()), &fences[0], static_cast<VkBool32>(waitForAll), timeout) != VK_SUCCESS)
	{
		LOG("[Graphics::fences] Waiting on fences failed!");
		return false;
	}
	return true;
}

bool VulkanFence::resetFences(std::shared_ptr<VulkanDevice> device, std::vector<VkFence> fences)
{
	if (!fences.empty() && vkResetFences(*device, static_cast<uint32_t>(fences.size()), &fences[0]) != VK_SUCCESS)
	{
		LOG("[Graphics::Fences] Failed to reset fences!");
		return false;
	}
	return true;
}
