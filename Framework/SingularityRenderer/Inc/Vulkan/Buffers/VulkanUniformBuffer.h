#pragma once
#include "Common.h"
#include "Vulkan/Memory/VulkanMemoryObject.h"
namespace SingularityEngine::SERenderer
{
	class VulkanUniformBuffer
	{
	public:
		VulkanUniformBuffer(VkDeviceSize size, VkBufferUsageFlags usage);
		~VulkanUniformBuffer();


		VkBuffer getBuffer() { return mBuffer; }
	private:
		std::unique_ptr<VulkanMemoryObject> mMemObject;
		VkDeviceSize mSize = 0;
		VkBuffer mBuffer = VK_NULL_HANDLE;
		VkDescriptorBufferInfo mDescriptor;
	};

}
