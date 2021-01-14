#pragma once
#include "Common.h"
#include "VulkanParameters.h"
#include "Instance.h"
namespace SingularityEngine::Vulkan
{
	class Device
	{
	public:
		Device();
		~Device();
		bool create(StartupParameters& startupInfo, Instance& instance);
		bool destroy();
		VkDevice& operator()() { return mLogicalDevice; }
	private:
		struct QueueInfo {
			uint32_t FamilyIndex;
			std::vector<float> Priorities;
		};
		bool checkAvailablePhysicalDevices(std::vector<VkPhysicalDevice>& availableDevices, Instance& instance);
		bool checkAvailableDevicesExtensions(VkPhysicalDevice physicalDevice, std::vector<VkExtensionProperties>& availableExtensions);
		bool checkPhysicalDeviceProperties(VkPhysicalDevice physicalDevice, StartupParameters& startupInfo);
		bool isExtensionSupported(const char* extensionName, const std::vector<VkExtensionProperties>& availableExtensions);
		bool selectQueueMatchingDesiredCapabilities(VkQueueFlags desiredCapabilites, uint32_t& queueIndex, std::vector<VkQueueFamilyProperties>& queueFamilyProperties);

		std::vector<VkPhysicalDevice> mPhysicalDevices;
		VkDevice mLogicalDevice = VK_NULL_HANDLE;
	};
}