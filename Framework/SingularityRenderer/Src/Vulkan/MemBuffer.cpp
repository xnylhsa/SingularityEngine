#include "Precompiled.h"
#include "Vulkan/MemBuffer.h"
#include "Vulkan/Util/VulkanFunctions.h"
#include "Vulkan/Device/VulkanDevice.h"
using namespace SingularityEngine::SERenderer;

MemBuffer::MemBuffer()
{

}

bool MemBuffer::create(std::weak_ptr<VulkanDevice> device, VkDeviceSize size, VkBufferUsageFlags usage)
{
	mDevice = device;
	auto vulkanDevice = mDevice.lock();
	ASSERT(vulkanDevice, "[SERenderer::VulkanImage] device lost.");
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

	if (vkCreateBuffer(*vulkanDevice, &createInfo, nullptr, &mBuffer) != VK_SUCCESS)
	{
		LOG("[Graphics::Buffer] Failed creation!");
		return false;
	}
	return true;
}

bool MemBuffer::destroy()
{
	auto vulkanDevice = mDevice.lock();
	ASSERT(vulkanDevice, "[SERenderer::VulkanImage] device lost.");
	if (vulkanDevice && mBuffer != VK_NULL_HANDLE)
	{
		vkDestroyBuffer(*vulkanDevice, mBuffer, nullptr);
		mBuffer = VK_NULL_HANDLE;
		mSize = 0;
	}
	return true;
}