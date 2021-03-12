#include "Precompiled.h"
#include "Vulkan/Memory/VulkanMemoryAllocator.h"
#include "Vulkan/Device/VulkanDevice.h"
#include "Vulkan/Util/VulkanFunctions.h"
#include "Vulkan/Memory/VulkanMemoryObject.h"
#include "RendererAPI/Renderer.h"
using namespace SingularityEngine::SERenderer;

VulkanMemoryAllocator::VulkanMemoryAllocator(std::shared_ptr<VulkanDevice> device, std::string tag) 
	: mTag(tag), rVulkanDevice(device)
{
}

VulkanMemoryAllocator::VulkanMemoryAllocator(std::string tag) 
	: mTag(tag), rVulkanDevice(std::dynamic_pointer_cast<VulkanDevice>(Renderer::Get()->getGraphicsDevice()))
{
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

	return std::move(std::make_unique<VulkanMemoryObject>(rVulkanDevice, dest));
}

std::unique_ptr<VulkanMemoryObject> VulkanMemoryAllocator::allocateBufferMemory(VkMemoryPropertyFlags memoryProperties, VkBuffer vkbuffer, bool bind)
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
			allocate(memoryRequirements, &dest, memoryProperties);
			break;
		}
	}

	if(!bind) return std::move(std::make_unique<VulkanMemoryObject>(rVulkanDevice, dest));
	if (vkBindBufferMemory(*rVulkanDevice, vkbuffer, dest, 0) != VK_SUCCESS)
	{
		LOG("[Graphics::Buffer] Could not bind memory object to buffer!");
		return nullptr;
	}
	return std::move(std::make_unique<VulkanMemoryObject>(rVulkanDevice, dest));
}
