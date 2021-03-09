#pragma once
#include "Common.h"

namespace SingularityEngine::SERenderer
{
	class VulkanDevice;
	class VulkanCommandPool
	{
	public:
		VulkanCommandPool();
		bool create(std::weak_ptr<VulkanDevice> device, uint32_t queueFamilyIndex, VkCommandBufferUsageFlags usageFlags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);
		bool destroy();
		bool reset(bool releaseResources);
		operator VkCommandPool() { ASSERT(mCommandPool != VK_NULL_HANDLE, "[Graphics::Vulkan] Command pool has not been initialized!"); return mCommandPool; }
	private:
		std::optional<uint32_t> mQueueFamilyIndex;
		VkCommandPool mCommandPool = VK_NULL_HANDLE;
		std::weak_ptr<VulkanDevice> mDevice;
	};
}