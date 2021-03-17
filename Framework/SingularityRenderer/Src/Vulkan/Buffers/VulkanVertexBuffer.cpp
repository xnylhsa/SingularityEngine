#include "Precompiled.h"
#include "Vulkan/Buffers/VulkanVertexBuffer.h"
#include "vulkan/Util/VulkanFunctions.h"
#include "vulkan/Device/VulkanDevice.h"
#include "vulkan/Memory/vk_mem_alloc.h"
#include "vulkan/SwapChain/VulkanSwapChain.h"
#include "RendererAPI/Renderer.h"
namespace SingularityEngine::SERenderer
{
	VulkanVertexBuffer::VulkanVertexBuffer(size_t size, VertexBufferUsage usageType) : mSize(size), mUsage(usageType)
	{

	}

	VulkanVertexBuffer::VulkanVertexBuffer(void* data, size_t size, VertexBufferUsage usageType) : mSize(size), mUsage(usageType)
	{
		std::shared_ptr<VulkanDevice> device = std::dynamic_pointer_cast<VulkanDevice>(Renderer::Get()->getGraphicsDevice());
		mData = Core::Buffer::Copy(data, (uint32_t)size);
		auto[buffer, memory] = device->requestBuffer(VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, mSize);
		mBuffer = buffer;
		mDeviceMemory = std::move(memory);

		void* deviceData = device->mapBuffer(&mDeviceMemory);
		memcpy(deviceData, mData.Data, size);
		device->unMapBuffer(&mDeviceMemory);
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
		device->releaseBuffer(mBuffer, std::move(mDeviceMemory));
	}

}