#pragma once
#include "Common.h"

namespace SingularityEngine::Vulkan
{
	class MemBuffer
	{
	public:
		MemBuffer();
		bool create(VkDevice& device, VkDeviceSize size, VkBufferUsageFlags usage);
		bool destroy(VkDevice& device);
		VkBuffer& get() { return mBuffer; }
	private:
		VkDeviceSize mSize = 0;
		VkBuffer mBuffer = VK_NULL_HANDLE;
	};
}