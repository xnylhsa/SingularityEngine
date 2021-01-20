#include "Precompiled.h"
#include "StorageTexelBuffer.h"
#include "VulkanFunctions.h"

using namespace SingularityEngine::Vulkan;

StorageTexelBuffer::StorageTexelBuffer(VkPhysicalDevice physicalDevice, VkDevice device, VkFormat format, VkDeviceSize size, VkBufferUsageFlags usage, bool useAtomicOperations)
{

	VkFormatProperties formatProperties;
	vkGetPhysicalDeviceFormatProperties(physicalDevice, format, &formatProperties);
	if (!(formatProperties.bufferFeatures & VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT))
	{
		ASSERT(false, "[Graphics::UniformTexelBuffer] Provided format is not supported with storage texel buffer!");
		return;
	}

	if (useAtomicOperations && !(formatProperties.bufferFeatures & VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_ATOMIC_BIT))
	{
		ASSERT(false, "[Graphics::UniformTexelBuffer] Provided format is not supported with storage texel buffer!");
		return;
	}

	mBuffer = new MemBuffer();
	mBuffer->create(device, size, usage | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT);
	ASSERT(mBuffer->isValid(), "[Graphics::UniformTexelBuffer] Failed to create Buffer!");
	mMemObject = new MemoryObject();
	ASSERT(mMemObject->createBufferMemoryObject(physicalDevice, device, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, mBuffer->get()), "[Graphics::UniformTexelBuffer] Failed to allocate and bind memory object");
	mBufferView = new BufferView();
	ASSERT(mBufferView->create(device, mBuffer->get(), format, 0, VK_WHOLE_SIZE), "[Graphics::UniformTexelBuffer] Failed to create buffer view!");
}

StorageTexelBuffer::~StorageTexelBuffer()
{
	SafeDelete(mBufferView);
	SafeDelete(mMemObject);
	SafeDelete(mBuffer);

}

bool StorageTexelBuffer::destroy(VkDevice device)
{
	return mBufferView->destroy(device) && mMemObject->destroy(device) && mBuffer->destroy(device);
}

bool StorageTexelBuffer::isValid()
{
	return mBuffer->isValid() && mMemObject->isValid() && mBuffer->isValid();
}
