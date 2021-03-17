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
		std::shared_ptr<VulkanDevice> device = std::dynamic_pointer_cast<VulkanDevice>(Renderer::Get()->getGraphicsDevice());
		mData = Core::Buffer::Copy(data, (uint32_t)size);
		auto [buffer, memory] = device->requestBuffer(VK_BUFFER_USAGE_INDEX_BUFFER_BIT, mSize);
		mBuffer = buffer;
		mDeviceMemory = std::move(memory);

		void* deviceData = device->mapBuffer(&mDeviceMemory);
		memcpy(deviceData, mData.Data, size);
		device->unMapBuffer(&mDeviceMemory);
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
		uint32_t count = (uint32_t)(mSize / sizeof(uint32_t));
		vkCmdDrawIndexed(swapchain->getCurrentCommandBuffer(), count, 1, 0, 0, 1); // this shouldnt be in the index buffer lol
	}

	size_t VulkanIndexBuffer::getSize() const
	{
		return mSize;
	}

	VulkanIndexBuffer::~VulkanIndexBuffer()
	{
		std::shared_ptr<VulkanDevice> device = std::dynamic_pointer_cast<VulkanDevice>(Renderer::Get()->getGraphicsDevice());
		device->releaseBuffer(mBuffer, std::move(mDeviceMemory));
	}

}