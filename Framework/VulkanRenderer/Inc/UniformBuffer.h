#pragma once
#include "Common.h"
#include "MemoryObject.h"
#include "MemBuffer.h"
#include "BufferView.h"
namespace SingularityEngine::Vulkan
{
	class UniformBuffer
	{
	public:
		UniformBuffer(VkPhysicalDevice physicalDevice, VkDevice device, VkDeviceSize size, VkBufferUsageFlags usage);
		~UniformBuffer();
		bool destroy(VkDevice device);
		bool isValid();
		VkBuffer getBuffer() { return mBuffer->get(); }
	private:
		MemoryObject* mMemObject;
		MemBuffer* mBuffer;
	};

}
