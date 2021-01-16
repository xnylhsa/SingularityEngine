#pragma once

namespace SingularityEngine::Vulkan
{
	class Fence
	{
	public:
		bool create(VkDevice device, bool signaled);
		bool destroy(VkDevice device);
		bool isInitialized() { return mInitialized; }
		VkFence get() { return mFence; }
	private:
		bool mInitialized = false;
		VkFence mFence;
	};
}