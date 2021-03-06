#pragma once
#include "Common.h"

namespace SingularityEngine::Graphics
{
	class VulkanMemoryObject
	{
	public:
		VulkanMemoryObject(VkDeviceMemory deviceMemory);
		~VulkanMemoryObject() { destroy(); }
		bool MapUpdateAndUnmapHostVisibleMemory(VkDeviceSize offset, VkDeviceSize dataSize, void* data, bool unmap = true, void** pointer = nullptr);
		bool isValid() { return mMemoryObject != VK_NULL_HANDLE; }
		operator VkDeviceMemory() { return mMemoryObject; }
	private:
		bool destroy();
		VkDeviceMemory mMemoryObject;
	};
}