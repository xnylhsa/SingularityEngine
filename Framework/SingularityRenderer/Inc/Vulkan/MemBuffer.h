#pragma once
#include "Common.h"

namespace SingularityEngine::SERenderer
{
	class VulkanDevice;
	class MemBuffer
	{
	public:
		MemBuffer();
		bool create(std::weak_ptr<VulkanDevice> device, VkDeviceSize size, VkBufferUsageFlags usage);
		bool destroy();
		VkBuffer& get() { return mBuffer; }
		bool isValid() { return mBuffer != VK_NULL_HANDLE; }
	private:
		VkDeviceSize mSize = 0;
		VkBuffer mBuffer = VK_NULL_HANDLE;
		std::weak_ptr<VulkanDevice> mDevice;
	};
}