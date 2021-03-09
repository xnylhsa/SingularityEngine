#pragma once
#include "Common.h"
#include "Vulkan/Memory/VulkanMemoryObject.h"
#include "MemBuffer.h"
#include "BufferView.h"
namespace SingularityEngine::SERenderer
{
	class UniformTexelBuffer
	{
	public:
		UniformTexelBuffer(VkPhysicalDevice physicalDevice, VkDevice device, VkFormat format, VkDeviceSize size, VkBufferUsageFlags usage);
		~UniformTexelBuffer();
		bool destroy(VkDevice device);
		bool isValid();

	private:
		VulkanMemoryObject* mMemObject;
		BufferView* mBufferView;
		MemBuffer* mBuffer;
	};

}