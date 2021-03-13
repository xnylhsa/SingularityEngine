#include "Precompiled.h"
#include "Vulkan/Buffers/VulkanVertexBuffer.h"
#include "vulkan/Util/VulkanFunctions.h"
#include "vulkan/Device/VulkanDevice.h"
#include "vulkan/Memory/VulkanMemoryAllocator.h"
#include "vulkan/SwapChain/VulkanSwapChain.h"
#include "RendererAPI/Renderer.h"
namespace SingularityEngine::SERenderer
{
	VulkanVertexBuffer::VulkanVertexBuffer(size_t size, VertexBufferUsage usageType) : mSize(size), mUsage(usageType)
	{

	}

	VulkanVertexBuffer::VulkanVertexBuffer(void* data, size_t size, VertexBufferUsage usageType) : mSize(size), mUsage(usageType)
	{
		mData = Core::Buffer::Copy(data, (uint32_t)size);

		VkBufferCreateInfo indexBufferInfo = {};
		indexBufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		indexBufferInfo.size = mSize;
		indexBufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;

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

	void VulkanVertexBuffer::setData(void* buffer, size_t size, size_t offset /*= 0*/)
	{
		UNREFERENCED_PARAMETER(buffer);
		UNREFERENCED_PARAMETER(size);
		UNREFERENCED_PARAMETER(offset);
	}

	void VulkanVertexBuffer::bind() const
	{
		std::shared_ptr<VulkanDevice> device = std::dynamic_pointer_cast<VulkanDevice>(Renderer::Get()->getGraphicsDevice());
		VulkanSwapChain* swapchain = dynamic_cast<VulkanSwapChain*>(Renderer::Get()->getSwapchain());
		VkDeviceSize offsets[1] = { 0 };
		vkCmdBindVertexBuffers(swapchain->getCurrentCommandBuffer(), 0, 1, &mBuffer, offsets);
	}

	size_t VulkanVertexBuffer::getSize() const
	{
		return mSize;
	}

	VulkanVertexBuffer::~VulkanVertexBuffer()
	{
		std::shared_ptr<VulkanDevice> device = std::dynamic_pointer_cast<VulkanDevice>(Renderer::Get()->getGraphicsDevice());

		mMemoryObject.reset();
		vkDestroyBuffer(device->getLogicalDevice(), mBuffer, nullptr);
	}

}