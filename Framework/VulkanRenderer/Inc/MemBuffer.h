#pragma once
#include "Common.h"

namespace SingularityEngine::Graphics
{
	class MemBuffer
	{
	public:
		MemBuffer();
		bool create(VkDeviceSize size, VkBufferUsageFlags usage);
		bool destroy();
		VkBuffer& get() { return mBuffer; }
		bool isValid() { return mBuffer != VK_NULL_HANDLE; }
	private:
		VkDeviceSize mSize = 0;
		VkBuffer mBuffer = VK_NULL_HANDLE;
	};
}