#include "Precompiled.h"
#include "Vulkan/Buffers/VulkanBuffer.h"
#include "vulkan/Util/VulkanFunctions.h"
#include "vulkan/Device/VulkanDevice.h"
#include "vulkan/Memory/VulkanMemoryAllocator.h"
#include "vulkan/SwapChain/VulkanSwapChain.h"
#include "RendererAPI/Renderer.h"
namespace SingularityEngine::SERenderer
{
	VulkanBuffer::VulkanBuffer(size_t size, size_t offset, BufferUsageType usageType) : mSize(size), mOffset(offset), mUsage(usageType)
	{
		UNREFERENCED_PARAMETER(usageType);
	}

	VulkanBuffer::VulkanBuffer(void* data, size_t size, size_t offset, BufferUsageType usageType) : mSize(size), mOffset(offset), mUsage(usageType)
	{
		mData = malloc(size);
		memcpy_s(mData, size, data, size);
		//mData = Buffer::copy(data, size);

		VkBufferCreateInfo indexBufferInfo = {};
		indexBufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		indexBufferInfo.size = mSize;
		indexBufferInfo.usage = convertUsageTypetoVKType(usageType);

		std::shared_ptr<VulkanDevice> device = std::dynamic_pointer_cast<VulkanDevice>(Renderer::Get()->getGraphicsDevice());
		ASSERT(vkCreateBuffer(device->getLogicalDevice(), &indexBufferInfo, nullptr, &mBuffer) == VK_SUCCESS, "[SERenderer::VulkanBuffer] failed creation!");
		VulkanMemoryAllocator allocator("VulkanBuffer");
		mMemoryObject = allocator.allocateBufferMemory(VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, mBuffer, false);

		void* destBuffer;
		if (vkMapMemory(device->getLogicalDevice(), mMemoryObject->getDeviceMemory(), 0, mSize, 0, &destBuffer) != VK_SUCCESS)
		{
			ASSERT(false, "");
		}
		memcpy(destBuffer, mData, mSize);
		vkUnmapMemory(device->getLogicalDevice(), mMemoryObject->getDeviceMemory());
		if (vkBindBufferMemory(*device, mBuffer, mMemoryObject->getDeviceMemory(), 0) != VK_SUCCESS)
		{
			LOG("[Graphics::Buffer] Could not bind memory object to buffer!");
			return;
		}
	}

	void VulkanBuffer::setData(void* buffer, size_t size, size_t offset /*= 0*/)
	{
		UNREFERENCED_PARAMETER(buffer);
		UNREFERENCED_PARAMETER(size);
		UNREFERENCED_PARAMETER(offset);
	}

	void VulkanBuffer::bind() const
	{
		std::shared_ptr<VulkanDevice> device = std::dynamic_pointer_cast<VulkanDevice>(Renderer::Get()->getGraphicsDevice());
		VulkanSwapChain* swapchain = dynamic_cast<VulkanSwapChain*>(Renderer::Get()->getSwapchain());

		switch (mUsage)
		{
		case SingularityEngine::SERenderer::BufferUsageType::VertexBuffer:
		{
			VkDeviceSize offsets[1] = { 0 };
			vkCmdBindVertexBuffers(swapchain->getCurrentCommandBuffer(), 0, 1, &mBuffer, offsets);
			break;
		}
		case SingularityEngine::SERenderer::BufferUsageType::IndexBuffer:
			vkCmdBindIndexBuffer(swapchain->getCurrentCommandBuffer(), mBuffer, {0}, VK_INDEX_TYPE_UINT32);
			vkCmdDrawIndexed(swapchain->getCurrentCommandBuffer(), 3, 1, 0, 0, 1);
			break;
		default:
			return;
		}
	}

	size_t VulkanBuffer::getCount() const
	{
		return (mSize / mOffset);
	}
	size_t VulkanBuffer::getSize() const
	{
		return mSize;
	}

	VkBufferUsageFlagBits VulkanBuffer::convertUsageTypetoVKType(BufferUsageType usageType)
	{
		switch (usageType)
		{
		case BufferUsageType::IndexBuffer:
			return VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
		case BufferUsageType::VertexBuffer:
			return VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
		}
		return VK_BUFFER_USAGE_FLAG_BITS_MAX_ENUM;
	}

	VulkanBuffer::~VulkanBuffer()
	{
		std::shared_ptr<VulkanDevice> device = std::dynamic_pointer_cast<VulkanDevice>(Renderer::Get()->getGraphicsDevice());

		mMemoryObject.reset();
		vkDestroyBuffer(device->getLogicalDevice(), mBuffer, nullptr);
		if (mData)
		{
			free(mData);
		}
	}

	size_t VulkanBuffer::getOffset() const
	{
		return mOffset;
	}

}