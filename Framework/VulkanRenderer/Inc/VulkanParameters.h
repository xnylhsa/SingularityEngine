#pragma once
#include "Common.h"

namespace SingularityEngine::Vulkan
{
	struct QueueParameters
	{
		uint32_t mQueueFamilyIndex = UINT32_MAX;
		VkQueueFlags desiredCapabilites = 0;
		std::vector<float> mQueuePriorities;
	};

	struct InstanceParameters
	{
		std::vector<const char*> mDesiredExtensions;
	};

	struct DeviceParameters
	{
		VkPhysicalDeviceFeatures mDesiredFeatures;
		std::vector<const char*> mDesiredDeviceExtensions;
	};

	struct StartupParameters
	{
		std::string mAppName = "";
		std::string mEngineName = "";
		uint32_t mMajorVersion = 0;
		uint32_t mMinorVersion = 0;
		uint32_t mPatchVersion = 0;
		uint32_t mEngineMajorVersion = 0;
		uint32_t mEngineMinorVersion = 0;
		uint32_t mEnginePatchVersion = 0;
		InstanceParameters mInstanceParameters;
		DeviceParameters mDeviceParameters;
		std::vector<QueueParameters> mQueueParameters;
	};
}