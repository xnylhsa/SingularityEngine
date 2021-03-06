#pragma once

namespace SingularityEngine::Graphics
{
	class VulkanFence
	{
	public:
		VulkanFence() = default;
		~VulkanFence() = default;
		void create(bool signaled);
		void destroy();
		static bool waitForFences(std::vector<VkFence> fences, bool waitForAll, uint64_t timeout);
		static bool resetFences(std::vector<VkFence> fences);

		operator VkFence() { return mFence; }
	private:
		VkFence mFence;
	};
}