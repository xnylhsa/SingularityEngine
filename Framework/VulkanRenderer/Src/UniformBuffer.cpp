#include "Precompiled.h"
#include "UniformBuffer.h"
#include "VulkanFunctions.h"

using namespace SingularityEngine::Vulkan;

UniformBuffer::UniformBuffer(VkPhysicalDevice physicalDevice, VkDevice device, VkDeviceSize size, VkBufferUsageFlags usage)
{
	mBuffer = new MemBuffer();
	mBuffer->create(device, size, usage | VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT);
	ASSERT(mBuffer->isValid(), "[Graphics::UniformTexelBuffer] Failed to create Buffer!");
	mMemObject = new MemoryObject();
	ASSERT(mMemObject->createBufferMemoryObject(physicalDevice, device, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, mBuffer->get()), "[Graphics::UniformTexelBuffer] Failed to allocate and bind memory object");
}

UniformBuffer::~UniformBuffer()
{
	SafeDelete(mMemObject);
	SafeDelete(mBuffer);
}

bool UniformBuffer::destroy(VkDevice device)
{
	return  mMemObject->destroy(device) && mBuffer->destroy(device);
}

bool UniformBuffer::isValid()
{
	return mMemObject->isValid() && mBuffer->isValid();
}
