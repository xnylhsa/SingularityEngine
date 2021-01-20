#pragma once
#include "Common.h"
#include "MemoryObject.h"
#include "MemBuffer.h"
#include "BufferView.h"
namespace SingularityEngine::Vulkan
{
	class UniformTexelBuffer
	{
	public:
		UniformTexelBuffer(VkPhysicalDevice physicalDevice, VkDevice device, VkFormat format, VkDeviceSize size, VkBufferUsageFlags usage);
		~UniformTexelBuffer();
		bool destroy(VkDevice device);
		bool isValid();

	private:
		MemoryObject* mMemObject;
		BufferView* mBufferView;
		MemBuffer* mBuffer;
	};

}