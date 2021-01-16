#pragma once
#include "Common.h"
#include "VulkanParameters.h"
#include "Instance.h"
#include "PresentationSurface.h"
#include "GraphicsQueue.h"
namespace SingularityEngine::Vulkan
{
	class Device
	{
	public:
		Device(PresentationSurface& presentSurface);
		~Device();
		bool create(StartupParameters& startupInfo, Instance& instance);
		bool createDeviceQueues();
		bool destroy();
		VkDevice& getLogicalDevice();
		VkPhysicalDevice& getPhysicalDevice();

		VkQueue& getGraphicsQueue() { return mGraphicsQueue.get(); }
		VkQueue& getComputeQueue() { return mComputeQueue.get(); }
		VkQueue& getPresentQueue() { return mPresentQueue.get(); }
		VkQueue& getTransferQueue() { return mTransferQueue.get(); }

	private:
		struct QueueInfo {
			uint32_t FamilyIndex;
			std::vector<float> Priorities;
		};
		struct QueueFamilyIndicies
		{
			std::optional<uint32_t> graphicsFamily;
			std::optional<uint32_t> presentFamily;
			std::optional<uint32_t> transferFamily;
			std::optional<uint32_t> computeFamily;

			bool isComplete()
			{
				return graphicsFamily.has_value() && presentFamily.has_value() && transferFamily.has_value() && computeFamily.has_value();
			}
		};

		struct PresentInfo {
			VkSwapchainKHR  swapchain;
			uint32_t        imageIndex;
		};

		bool checkAvailablePhysicalDevices(std::vector<VkPhysicalDevice>& availableDevices, Instance& instance);
		bool checkAvailableDevicesExtensions(VkPhysicalDevice physicalDevice, std::vector<VkExtensionProperties>& availableExtensions);
		uint32_t ratePhysicalDeviceProperties(VkPhysicalDevice physicalDevice, StartupParameters& startupInfo, QueueFamilyIndicies& indicies);
		bool isExtensionSupported(const char* extensionName, const std::vector<VkExtensionProperties>& availableExtensions);
		bool selectQueueMatchingDesiredCapabilities(VkPhysicalDevice device, QueueFamilyIndicies& indicies);



		VkSurfaceKHR& rPresentationSurface;
		GraphicsQueueFamily mPresentQueue;
		GraphicsQueueFamily mTransferQueue;
		GraphicsQueueFamily mGraphicsQueue;
		GraphicsQueueFamily mComputeQueue;
		VkPhysicalDevice mPhysicalDevice = VK_NULL_HANDLE;
		VkDevice mLogicalDevice = VK_NULL_HANDLE;
		std::vector<VkImage> mSwapChainImages;

		bool mCanRender = false;
	};
}