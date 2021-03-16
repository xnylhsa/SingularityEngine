#pragma once
#include "Common.h"

namespace SingularityEngine::SERenderer
{
	class VulkanDevice;
	class VulkanCommandPool
	{
	public:

		VulkanCommandPool();
		VulkanCommandPool(std::weak_ptr<VulkanDevice> device, uint32_t queueFamilyIndex);

		bool create(std::weak_ptr<VulkanDevice> device, uint32_t queueFamilyIndex); // should make this RAII
		bool destroy(); // should make this RAII
		VulkanCommandPool(VulkanCommandPool&& other) noexcept;
		VulkanCommandPool& operator=(VulkanCommandPool&& other) noexcept;
		VulkanCommandPool(const VulkanCommandPool& other) = delete;
		void operator=(const VulkanCommandPool& other) = delete;
		VkCommandBuffer requestCommandBuffer();
		VkCommandBuffer requestSecondaryCommandBuffer();
		void begin();
		void trim();

		operator VkCommandPool() { ASSERT(mCommandPool != VK_NULL_HANDLE, "[Graphics::Vulkan] Command pool has not been initialized!"); return mCommandPool; }
	private:
		VkCommandBuffer allocateCommandBuffer(bool isSecondary = false);

		std::optional<uint32_t> mQueueFamilyIndex;
		std::vector<VkCommandBuffer> mBuffers;
		std::vector<VkCommandBuffer> mSecondaryBuffers;
		VkCommandPool mCommandPool = VK_NULL_HANDLE;
		std::weak_ptr<VulkanDevice> mDevice;
		uint32_t mIndex;
		uint32_t mSecondaryIndex;
	};
}