#pragma once
#include "Common.h"
#include "MemoryObject.h"
#include "MemBuffer.h"
#include "BufferView.h"
namespace SingularityEngine::Vulkan
{
	class StorageBuffer
	{
	public:
		StorageBuffer(VkPhysicalDevice physicalDevice, VkDevice device, VkDeviceSize size, VkBufferUsageFlags usage);
		~StorageBuffer();
		bool destroy(VkDevice device);
		bool isValid();

	private:
		MemoryObject* mMemObject;
		MemBuffer* mBuffer;
	};

}
#pragma once
