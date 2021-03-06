#pragma once
#include "Common.h"
#include "VulkanParameters.h"
#include "VulkanQueue.h"
#include "VulkanCommandPool.h"
namespace SingularityEngine::Graphics
{
	class VulkanDevice
	{
	public:
		VulkanDevice(StartupParameters& startupInfo);
		~VulkanDevice();
		bool createDeviceQueues();
		bool destroyDeviceQueues();
		bool wait();
		operator VkDevice();
		operator VkPhysicalDevice();
		bool isValid() { return mPhysicalDevice != VK_NULL_HANDLE && mLogicalDevice != VK_NULL_HANDLE; }

		uint32_t getMemoryTypeIndex(uint32_t typeBits, VkMemoryPropertyFlags properties);

		VulkanQueue& getGraphicsQueue() { return mGraphicsQueue; }
		VkFormat findFirstCompatibleDepthFormat(std::vector<VkFormat> depthFormats);
	private:
		struct QueueInfo
		{
			uint32_t queueFamilyIndex;
			std::vector<float> priorities;
		};
		struct QueueFamilyIndicies
		{
			std::optional<uint32_t> graphicsFamily;
			std::optional<uint32_t> transferFamily;
			std::optional<uint32_t> computeFamily;
			bool isComplete()
			{
				return graphicsFamily.has_value() && transferFamily.has_value() && computeFamily.has_value();
			}
		};

		bool checkAvailablePhysicalDevices(std::vector<VkPhysicalDevice>& availableDevices);
		bool checkAvailableDevicesExtensions(VkPhysicalDevice physicalDevice, std::vector<VkExtensionProperties>& availableExtensions);
		uint32_t ratePhysicalDeviceProperties(VkPhysicalDevice physicalDevice, StartupParameters& startupInfo);
		bool isExtensionSupported(const char* extensionName, const std::vector<VkExtensionProperties>& availableExtensions);
		bool selectQueueMatchingDesiredCapabilities(VkPhysicalDevice device);

		bool setPhysicalDeviceInfo();
		VkPhysicalDevice mPhysicalDevice = VK_NULL_HANDLE;
		VkPhysicalDeviceMemoryProperties mMemoryProperties;
		VkPhysicalDeviceProperties mDeviceProperties;
		VkPhysicalDeviceFeatures mDeviceFeatures;
		QueueFamilyIndicies mQueueFamilyIndicies;
		std::vector<VkExtensionProperties> mPhysicalDeviceExtentions;
		VkDevice mLogicalDevice = VK_NULL_HANDLE;
		VulkanCommandPool mCommandPool;
		VulkanQueue mGraphicsQueue;
	};
}