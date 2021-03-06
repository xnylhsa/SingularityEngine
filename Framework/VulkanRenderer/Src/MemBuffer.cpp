#include "Precompiled.h"
#include "MemBuffer.h"
#include "VulkanFunctions.h"
#include "VulkanContext.h"
using namespace SingularityEngine::Graphics;

MemBuffer::MemBuffer()
{

}

bool MemBuffer::create(VkDeviceSize size, VkBufferUsageFlags usage)
{
	std::shared_ptr<VulkanDevice> device = VulkanContext::GetVulkanDevice();

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

	if (vkCreateBuffer(*device, &createInfo, nullptr, &mBuffer) != VK_SUCCESS)
	{
		LOG("[Graphics::Buffer] Failed creation!");
		return false;
	}
	return true;
}

bool MemBuffer::destroy()
{
	if (mBuffer != VK_NULL_HANDLE)
	{
		std::shared_ptr<VulkanDevice> device = VulkanContext::GetVulkanDevice();
		vkDestroyBuffer(*device, mBuffer, nullptr);
		mBuffer = VK_NULL_HANDLE;
		mSize = 0;
	}
	return true;
}