#include "Precompiled.h"
#include "Vulkan/Buffers/VulkanIndexBuffer.h"
#include "vulkan/Util/VulkanFunctions.h"
#include "vulkan/Device/VulkanDevice.h"
#include "vulkan/Memory/VulkanMemoryAllocator.h"
#include "vulkan/SwapChain/VulkanSwapChain.h"
#include "RendererAPI/Renderer.h"
namespace SingularityEngine::SERenderer
{
	VulkanIndexBuffer::VulkanIndexBuffer(size_t size) : mSize(size)
	{

	}

	VulkanIndexBuffer::VulkanIndexBuffer(void* data, size_t size) : mSize(size)
	{
		mData = Core::Buffer::Copy(data, (uint32_t)size);

		VkBufferCreateInfo indexBufferInfo = {};
		indexBufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		indexBufferInfo.size = mSize;
		indexBufferInfo.usage = VK_BUFFER_USAGE_INDEX_BUFFER_BIT;

		std::shared_ptr<VulkanDevice> device = std::dynamic_pointer_cast<VulkanDevice>(Renderer::Get()->getGraphicsDevice());
		ASSERT(vkCreateBuffer(device->getLogicalDevice(), &indexBufferInfo, nullptr, &mBuffer) == VK_SUCCESS, "[SERenderer::VulkanBuffer] failed creation!");
		VulkanMemoryAllocator allocator("VulkanBuffer");
		mMemoryObject = allocator.allocateBufferMemory(VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, mBuffer, false);

		void* destBuffer;
		if (vkMapMemory(device->getLogicalDevice(), mMemoryObject->getDeviceMemory(), 0, mSize, 0, &destBuffer) != VK_SUCCESS)
		{
			ASSERT(false, "");
		}
		memcpy(destBuffer, mData.Data, (uint32_t)mSize);
		vkUnmapMemory(device->getLogicalDevice(), mMemoryObject->getDeviceMemory());
		if (vkBindBufferMemory(*device, mBuffer, mMemoryObject->getDeviceMemory(), 0) != VK_SUCCESS)
		{
			LOG("[Graphics::Buffer] Could not bind memory object to buffer!");
			return;
		}
	}

	void VulkanIndexBuffer::setData(void* buffer, size_t size, size_t offset /*= 0*/)
	{
		UNREFERENCED_PARAMETER(buffer);
		UNREFERENCED_PARAMETER(size);
		UNREFERENCED_PARAMETER(offset);
	}

	void VulkanIndexBuffer::bind() const
	{
		std::shared_ptr<VulkanDevice> device = std::dynamic_pointer_cast<VulkanDevice>(Renderer::Get()->getGraphicsDevice());
		auto* swapchain = dynamic_cast<VulkanSwapChain*>(Renderer::Get()->getSwapchain());

		vkCmdBindIndexBuffer(swapchain->getCurrentCommandBuffer(), mBuffer, {0}, VK_INDEX_TYPE_UINT32);
		vkCmdDrawIndexed(swapchain->getCurrentCommandBuffer(), 3, 1, 0, 0, 1); // this shouldnt be in the index buffer lol
	}

	size_t VulkanIndexBuffer::getSize() const
	{
		return mSize;
	}

	VulkanIndexBuffer::~VulkanIndexBuffer()
	{
		std::shared_ptr<VulkanDevice> device = std::dynamic_pointer_cast<VulkanDevice>(Renderer::Get()->getGraphicsDevice());

		mMemoryObject.reset();
		vkDestroyBuffer(device->getLogicalDevice(), mBuffer, nullptr);
	}

}