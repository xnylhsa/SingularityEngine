#include "precompiled.h"
#include "BufferView.h"
#include "VulkanFunctions.h"


using namespace SingularityEngine::Graphics;

bool BufferView::create(VkDevice device, VkBuffer buffer, VkFormat format, VkDeviceSize memoryOffset, VkDeviceSize memoryRange)
{
	VkBufferViewCreateInfo bufferViewCreateInfo = {
		VK_STRUCTURE_TYPE_BUFFER_VIEW_CREATE_INFO,
		nullptr,
		0,
		buffer,
		format,
		memoryOffset,
		memoryRange
	};

	if (vkCreateBufferView(device, &bufferViewCreateInfo, nullptr, &mBufferView) != VK_SUCCESS)
	{
		return false;
	}
	return true;
}

bool BufferView::destroy(VkDevice device)
{
	if (mBufferView != VK_NULL_HANDLE)
	{
		vkDestroyBufferView(device, mBufferView, nullptr);
		mBufferView = VK_NULL_HANDLE;
	}
	return true;
}
