#pragma once
#include "Common.h"

namespace SingularityEngine::Vulkan
{

	struct InstanceParameters
	{
		std::vector<const char*> mDesiredExtensions;
	};

	struct DeviceParameters
	{
		VkPhysicalDeviceFeatures mDesiredFeatures;
		std::vector<const char*> mDesiredDeviceExtensions;
	};
	struct WindowParams
	{
		HINSTANCE mAppInstance;
		HWND mWindowHandle;
		uint32_t width;
		uint32_t height;
	};

	struct StartupParameters
	{
		std::string mAppName = "";
		std::string mEngineName = "";
		WindowParams windowParams;
		uint32_t mMajorVersion = 0;
		uint32_t mMinorVersion = 0;
		uint32_t mPatchVersion = 0;
		uint32_t mEngineMajorVersion = 0;
		uint32_t mEngineMinorVersion = 0;
		uint32_t mEnginePatchVersion = 0;
		InstanceParameters mInstanceParameters;
		DeviceParameters mDeviceParameters;
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