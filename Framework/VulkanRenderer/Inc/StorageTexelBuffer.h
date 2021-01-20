#pragma once
#include "Common.h"
#include "MemoryObject.h"
#include "MemBuffer.h"
#include "BufferView.h"
namespace SingularityEngine::Vulkan
{
	class StorageTexelBuffer
	{
	public:
		StorageTexelBuffer(VkPhysicalDevice physicalDevice, VkDevice device, VkFormat format, VkDeviceSize size, VkBufferUsageFlags usage, bool useAtomicOperations = false);
		~StorageTexelBuffer();
		bool destroy(VkDevice device);
		bool isValid();

	private:
		MemoryObject* mMemObject;
		BufferView* mBufferView;
		MemBuffer* mBuffer;
	};

}
