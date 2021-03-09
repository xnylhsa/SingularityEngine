#pragma once
#include "Common.h"

namespace SingularityEngine::SERenderer
{
	class BufferView
	{
	public:
		bool create(VkDevice device, VkBuffer buffer, VkFormat format, VkDeviceSize memoryOffset, VkDeviceSize memoryRange);
		bool destroy(VkDevice device);
	private:
		VkBufferView mBufferView;
	};
}