#pragma once

namespace SingularityEngine::SERenderer
{
	class VulkanDevice;
	class VulkanFence
	{
	public:
		VulkanFence() = default;
		~VulkanFence() = default;
		void create(std::weak_ptr<VulkanDevice> device, bool signaled);
		void destroy();
		static bool waitForFences(std::shared_ptr<VulkanDevice> device, std::vector<VkFence> fences, bool waitForAll, uint64_t timeout);
		static bool resetFences(std::shared_ptr<VulkanDevice> device, std::vector<VkFence> fences);

		operator VkFence() { return mFence; }
	private:
		VkFence mFence;
		std::weak_ptr<VulkanDevice> mDevice;
	};
}