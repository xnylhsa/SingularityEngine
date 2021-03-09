#pragma once
#include "Common.h"

namespace SingularityEngine::SERenderer
{
	using DesiredExtensions = std::vector<const char*>;

	struct WaitSemaphoreInfo
	{
		VkSemaphore semaphore = VK_NULL_HANDLE;
		VkPipelineStageFlags waitingStage = VK_NULL_HANDLE;
	};


	struct BufferTransition 
	{
		VkBuffer buffer;
		VkAccessFlags currentAccess;
		VkAccessFlags newAccess;
		uint32_t currentQueueFamily;
		uint32_t newQueueFamily;
	};

	struct ImageTransition {
		VkImage             image;
		VkAccessFlags       currentAccess;
		VkAccessFlags       newAccess;
		VkImageLayout       currentLayout;
		VkImageLayout       newLayout;
		uint32_t            currentQueueFamily;
		uint32_t            newQueueFamily;
		VkImageAspectFlags  aspect;
	};

}