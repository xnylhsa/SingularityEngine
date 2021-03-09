#pragma once
#include "Common.h"
#include "Vulkan/Memory/VulkanMemoryObject.h"
#include "MemBuffer.h"
#include "BufferView.h"
namespace SingularityEngine::SERenderer
{
	class StorageTexelBuffer
	{
	public:
		StorageTexelBuffer(VkPhysicalDevice physicalDevice, VkDevice device, VkFormat format, VkDeviceSize size, VkBufferUsageFlags usage, bool useAtomicOperations = false);
		~StorageTexelBuffer();
		bool destroy(VkDevice device);
		bool isValid();

	private:
		VulkanMemoryObject* mMemObject;
		BufferView* mBufferView;
		MemBuffer* mBuffer;
	};

}
