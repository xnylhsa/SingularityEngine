#pragma once
#include "Common.h"

namespace SingularityEngine::SERenderer
{
	class VulkanDevice;
	class VulkanMemoryObject
	{
	public:
		VulkanMemoryObject(std::weak_ptr<VulkanDevice> device, VkDeviceMemory deviceMemory);
		~VulkanMemoryObject() { destroy(); }
		bool MapUpdateAndUnmapHostVisibleMemory(VkDeviceSize offset, VkDeviceSize dataSize, void* data, bool unmap = true, void** pointer = nullptr);
		bool isValid() { return mMemoryObject != VK_NULL_HANDLE; }
		operator VkDeviceMemory() { return mMemoryObject; }
	private:
		bool destroy();
		VkDeviceMemory mMemoryObject;
		std::weak_ptr<VulkanDevice> mDevice;
	};
}