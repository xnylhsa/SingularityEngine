#include "Precompiled.h"
#include "Vulkan/Buffers/VulkanUniformBuffer.h"
#include "vulkan/util/VulkanFunctions.h"
#include "vulkan/Device/vulkanDevice.h"
#include "RendererAPI/Renderer.h"
#include "vulkan/memory/VulkanMemoryAllocator.h"
#include "vulkan/Memory/VulkanMemoryObject.h"
using namespace SingularityEngine::SERenderer;

void VulkanUniformBuffer::allocateBuffer()
{
	std::shared_ptr<VulkanDevice> device = std::dynamic_pointer_cast<VulkanDevice>(Renderer::Get()->getGraphicsDevice());
	VkBufferCreateInfo createInfo = {
		VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
		nullptr,
		0,
		mSize,
		VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
		VK_SHARING_MODE_EXCLUSIVE,
		0,
		nullptr
	};

	if (vkCreateBuffer(device->getLogicalDevice(), &createInfo, nullptr, &mBuffer) != VK_SUCCESS)
	{
		ASSERT(false, "[Graphics::Buffer] Failed creation!");
		return;
	}
	VulkanMemoryAllocator allocator("Uniform Buffer");
	mMemObject = allocator.allocateBufferMemory(VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, mBuffer);
	mDescriptor.buffer = mBuffer;
	mDescriptor.offset = 0;
	mDescriptor.range = sizeof(mSize);
}

VulkanUniformBuffer::~VulkanUniformBuffer()
{
	releaseBuffer();
}

void* VulkanUniformBuffer::map()
{
	std::shared_ptr<VulkanDevice> device = std::dynamic_pointer_cast<VulkanDevice>(Renderer::Get()->getGraphicsDevice());
	uint8_t* pData;
	ASSERT(vkMapMemory(device->getLogicalDevice(), mMemObject->getDeviceMemory(), 0, mSize, 0, (void**)&pData), "[SERenderer::UniformBuffer] failed to map memory");
	return pData;
}

void VulkanUniformBuffer::unmap()
{
	std::shared_ptr<VulkanDevice> device = std::dynamic_pointer_cast<VulkanDevice>(Renderer::Get()->getGraphicsDevice());
	vkUnmapMemory(device->getLogicalDevice(), mMemObject->getDeviceMemory());
}

void VulkanUniformBuffer::releaseBuffer()
{
	std::shared_ptr<VulkanDevice> device = std::dynamic_pointer_cast<VulkanDevice>(Renderer::Get()->getGraphicsDevice());
	ASSERT(device, "[SERenderer::VulkanImage] device lost.");
	if (device && mBuffer != VK_NULL_HANDLE)
	{
		vkDestroyBuffer(*device, mBuffer, nullptr);
		mBuffer = VK_NULL_HANDLE;
		mSize = 0;
	}
	mMemObject.reset();
}
