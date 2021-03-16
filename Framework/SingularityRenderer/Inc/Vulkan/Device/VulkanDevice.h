#pragma once
#include "Common.h"
#include "RendererAPI/IGraphicsDevice.h"
#include "Vulkan/Util/VulkanParameters.h"
#include "Vulkan/Device/VulkanQueue.h"
#include "Vulkan/Command/VulkanCommandPool.h"
#include "vulkan/Memory/vk_mem_alloc.h"
#include "vulkan/Texture/VulkanSampler.h"
#include "vulkan/DescriptorSets/VulkanDescriptorSet.h"
namespace SingularityEngine::SERenderer
{
	class VulkanDescriptorSetAllocator;
	class VulkanMemoryAllocator;
	class VulkanDevice : public IGraphicsDevice
	{
	public:
		VulkanDevice();
		bool createDeviceQueues();
		bool destroyDeviceQueues();
		bool waitUntilIdle();
		operator VkDevice();
		operator VkPhysicalDevice();
		VkDevice getLogicalDevice() { return mLogicalDevice; }
		VkPhysicalDevice getPhysicalDevice() { return mPhysicalDevice; }

		bool isValid() { return mPhysicalDevice != VK_NULL_HANDLE && mLogicalDevice != VK_NULL_HANDLE; }

		uint32_t getMemoryTypeIndex(uint32_t typeBits, VkMemoryPropertyFlags properties);

		VulkanQueue& getGraphicsQueue() { return mGraphicsQueue; }
		VkFormat findFirstCompatibleDepthFormat(std::vector<VkFormat> depthFormats);

		bool initialize() override;


		void teardown() override;
		std::shared_ptr<VulkanSampler> getStockSampler(SamplerTypes sampler);


		VkCommandBuffer getCommandBuffer(bool begin, bool compute = false);
		//TODO setup command buffer class, then have second version of this function that gets a profiled version.
		void submit(VkCommandBuffer commandBuffer);
		void submit(VkCommandBuffer commandBuffer, VkQueue queue);
		VkImage createImage();


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
		uint32_t ratePhysicalDeviceProperties(VkPhysicalDevice physicalDevice, DesiredExtensions& desiredExtensions);
		bool isExtensionSupported(const char* extensionName, const std::vector<VkExtensionProperties>& availableExtensions);
		bool selectQueueMatchingDesiredCapabilities(VkPhysicalDevice device);

		void createStockSamplers();
		void destroyStockSamplers();

		void initBindless();

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
		VmaAllocator mVMAAllocator;
		bool mSupportsUnboundedArrays;
		std::array<std::shared_ptr<VulkanSampler>, static_cast<size_t>(SamplerTypes::Count)> mStockSamplers;

		std::unique_ptr<VulkanMemoryAllocator> mAllocator;
		std::unique_ptr<VulkanDescriptorSetAllocator> mBindlesDescripterSetAllocatorInt;
		std::unique_ptr<VulkanDescriptorSetAllocator> mBindlesDescripterSetAllocatorFloat;

	};
}