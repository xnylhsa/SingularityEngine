#include "Precompiled.h"
////#include "StorageBuffer.h"
////#include "VulkanFunctions.h"
////
////using namespace SingularityEngine::Graphics;
////
////StorageBuffer::StorageBuffer(VkPhysicalDevice physicalDevice, VkDevice device, VkDeviceSize size, VkBufferUsageFlags usage)
////{
////	mBuffer = new MemBuffer();
////	mBuffer->create(device, size, usage | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT);
////	ASSERT(mBuffer->isValid(), "[Graphics::UniformTexelBuffer] Failed to create Buffer!");
////	mMemObject = new VulkanMemoryObject();
////	ASSERT(mMemObject->createBufferMemoryObject(physicalDevice, device, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, mBuffer->get()), "[Graphics::UniformTexelBuffer] Failed to allocate and bind memory object");
////}
////
////StorageBuffer::~StorageBuffer()
////{
////	SafeDelete(mMemObject);
////	SafeDelete(mBuffer);
////}
////
////bool StorageBuffer::destroy(VkDevice device)
////{
////	return  mMemObject->destroy(device) && mBuffer->destroy(device);
////}
////
////bool StorageBuffer::isValid()
////{
////	return mMemObject->isValid() && mBuffer->isValid();
////}
