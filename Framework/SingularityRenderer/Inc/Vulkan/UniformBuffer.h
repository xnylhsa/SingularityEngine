#pragma once
#include "Common.h"
#include "Vulkan/Memory/VulkanMemoryObject.h"
#include "MemBuffer.h"
#include "BufferView.h"
namespace SingularityEngine::SERenderer
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
		VulkanMemoryObject* mMemObject;
		MemBuffer* mBuffer;
	};

}
