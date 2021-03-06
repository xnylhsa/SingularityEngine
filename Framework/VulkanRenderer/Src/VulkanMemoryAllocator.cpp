#include "Precompiled.h"
#include "VulkanMemoryAllocator.h"
#include "VulkanDevice.h"
#include "VulkanContext.h"
#include "VulkanFunctions.h"
#include "VulkanMemoryObject.h"
using namespace SingularityEngine::Graphics;

VulkanMemoryAllocator::VulkanMemoryAllocator(std::string tag) : mTag(tag)
{
	rVulkanDevice = VulkanContext::GetVulkanDevice();
}

VulkanMemoryAllocator::~VulkanMemoryAllocator()
{

}

void VulkanMemoryAllocator::allocate(VkMemoryRequirements requirements, VkDeviceMemory* dest, VkMemoryPropertyFlags flags /*= VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT*/)
{
	LOG("Vulkan Allocator %s: allocating %llu bytes", mTag.c_str(), static_cast<uint64_t>(requirements.size));

	VkMemoryAllocateInfo memAlloc{};
	memAlloc.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	memAlloc.allocationSize = requirements.size;
	memAlloc.memoryTypeIndex = rVulkanDevice->getMemoryTypeIndex(requirements.memoryTypeBits, flags);
	vkAllocateMemory(*rVulkanDevice, &memAlloc, nullptr, dest);
}

std::unique_ptr<VulkanMemoryObject> VulkanMemoryAllocator::allocateImageMemory(VkMemoryPropertyFlags memoryProperties, VkImage image)
{
 	VkDeviceMemory dest = nullptr;
	VkPhysicalDeviceMemoryProperties physicalDeviceMemProps;
	vkGetPhysicalDeviceMemoryProperties(*rVulkanDevice, &physicalDeviceMemProps);

	VkMemoryRequirements memoryRequirements;
	vkGetImageMemoryRequirements(*rVulkanDevice, image, &memoryRequirements);

	for (uint32_t type = 0; type < physicalDeviceMemProps.memoryTypeCount; ++type)
	{
		if ((memoryRequirements.memoryTypeBits & (1 << type)) && (physicalDeviceMemProps.memoryTypes[type].propertyFlags & memoryProperties) == memoryProperties)
		{
			VkMemoryAllocateInfo bufferMemoryAllocateInfo = {
				VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
				nullptr,
				memoryRequirements.size,
				type
			};

			allocate(memoryRequirements, &dest);
		}
	}

	if (vkBindImageMemory(*rVulkanDevice, image, dest, 0) != VK_SUCCESS)
	{
		LOG("[Graphics::Buffer] Could not bind memory object to buffer!");
		return nullptr;
	}

	return std::move(std::make_unique<VulkanMemoryObject>(dest));
}

std::unique_ptr<VulkanMemoryObject> VulkanMemoryAllocator::allocateBufferMemory(VkMemoryPropertyFlags memoryProperties, VkBuffer vkbuffer)
{
	VkDeviceMemory dest = nullptr;
	VkPhysicalDeviceMemoryProperties physicalDeviceMemProps;
	vkGetPhysicalDeviceMemoryProperties(*rVulkanDevice, &physicalDeviceMemProps);

	VkMemoryRequirements memoryRequirements;
	vkGetBufferMemoryRequirements(*rVulkanDevice, vkbuffer, &memoryRequirements);

	for (uint32_t type = 0; type < physicalDeviceMemProps.memoryTypeCount; ++type)
	{
		if ((memoryRequirements.memoryTypeBits & (1 << type)) && (physicalDeviceMemProps.memoryTypes[type].propertyFlags & memoryProperties) == memoryProperties)
		{
			allocate(memoryRequirements, &dest);
		}
	}

	if (vkBindBufferMemory(*rVulkanDevice, vkbuffer, dest, 0) != VK_SUCCESS)
	{
		LOG("[Graphics::Buffer] Could not bind memory object to buffer!");
		return nullptr;
	}
	return std::move(std::make_unique<VulkanMemoryObject>(dest));
}
