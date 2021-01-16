#pragma once
#include "Common.h"

namespace SingularityEngine::Vulkan
{
	class CommandPool
	{
	public:
		CommandPool();
		bool create(VkDevice& device, uint32_t queueFamilyIndex);
		bool destroy(VkDevice& device);
		bool reset(VkDevice& device, bool releaseResources);
	private:
		std::optional<uint32_t> mQueueFamilyIndex;
		VkCommandPool mCommandPool = VK_NULL_HANDLE;
	};
}