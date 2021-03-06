#pragma once
#include "Common.h"

namespace SingularityEngine::Graphics
{
	class VulkanDevice;
	class VulkanMemoryObject;
	class VulkanMemoryAllocator
	{
	public:
		VulkanMemoryAllocator(std::string tag);
		~VulkanMemoryAllocator();
		void allocate(VkMemoryRequirements requirements, VkDeviceMemory* dest, VkMemoryPropertyFlags flags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
		std::unique_ptr<VulkanMemoryObject> allocateImageMemory(VkMemoryPropertyFlags memoryProperties, VkImage image);
		std::unique_ptr<VulkanMemoryObject> allocateBufferMemory(VkMemoryPropertyFlags memoryProperties, VkBuffer vkbuffer);
	private:
		std::shared_ptr<VulkanDevice> rVulkanDevice = nullptr;
		std::string mTag;
	};
}