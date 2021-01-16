#include "Precompiled.h"
#include "MemBuffer.h"
#include "VulkanFunctions.h"

using namespace SingularityEngine::Vulkan;

MemBuffer::MemBuffer()
{

}

bool MemBuffer::create(VkDevice& device, VkDeviceSize size, VkBufferUsageFlags usage)
{
	mSize = size;
	VkBufferCreateInfo createInfo = {
		VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
		nullptr,
		0,
		mSize,
		usage,
		VK_SHARING_MODE_EXCLUSIVE,
		0,
		nullptr
	};

	if (vkCreateBuffer(device, &createInfo, nullptr, &mBuffer) != VK_SUCCESS)
	{
		LOG("[Graphics::Buffer] Failed creation!");
		return false;
	}
	return true;
}

bool MemBuffer::destroy(VkDevice& device)
{
	if (mBuffer != VK_NULL_HANDLE)
	{
		vkDestroyBuffer(device, mBuffer, nullptr);
		mBuffer = VK_NULL_HANDLE;
		mSize = 0;
	}
	return true;
}