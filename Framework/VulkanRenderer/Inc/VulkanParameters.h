#pragma once
#include "Common.h"

namespace SingularityEngine::Graphics
{
	using DesiredExtensions = std::vector<const char*>;

	struct StartupParameters
	{
		Core::Window* mWindow = nullptr;
		uint32_t mMajorVersion = 0;
		uint32_t mMinorVersion = 0;
		uint32_t mPatchVersion = 0;
		std::string mAppName;
		VkPhysicalDeviceFeatures mDesiredDeviceFeatures;
		DesiredExtensions mInstanceExtensions;
		DesiredExtensions mDeviceExtensions;
	};

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