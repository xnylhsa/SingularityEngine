#include "Precompiled.h"
#include "Vulkan/Synchronization/VulkanCommandPool.h"
#include "Vulkan/Util/VulkanFunctions.h"
#include "Vulkan/Device/VulkanDevice.h"
using namespace SingularityEngine::SERenderer;


VulkanCommandPool::VulkanCommandPool()
{

}

bool VulkanCommandPool::create(std::weak_ptr<VulkanDevice> device, uint32_t queueFamilyIndex, VkCommandBufferUsageFlags usageFlags)
{
	mDevice = device;
	auto vulkanDevice = mDevice.lock();
	ASSERT(vulkanDevice, "[SERenderer::VulkanImage] device lost.");
	mQueueFamilyIndex = queueFamilyIndex;
	VkCommandPoolCreateFlags parameters = usageFlags;
	VkCommandPoolCreateInfo cmdPoolCreateInfo = {
  VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
  nullptr,
  parameters,
  *mQueueFamilyIndex
	};

	if (vkCreateCommandPool(*vulkanDevice, &cmdPoolCreateInfo, nullptr, &mCommandPool) != VK_SUCCESS)
	{
		LOG("[Graphics::CommandPool] Failed creation.");
		return false;
	}

	return true;
}

bool VulkanCommandPool::destroy()
{
	auto vulkanDevice = mDevice.lock();
	ASSERT(vulkanDevice, "[SERenderer::VulkanImage] device lost.");
	if (vulkanDevice && VK_NULL_HANDLE != mCommandPool) {
		vkDestroyCommandPool(*vulkanDevice, mCommandPool, nullptr);
		mCommandPool = VK_NULL_HANDLE;
		mDevice.reset();
	}
	return true;
}

bool VulkanCommandPool::reset(bool releaseResources)
{
	auto vulkanDevice = mDevice.lock();
	ASSERT(vulkanDevice, "[SERenderer::VulkanImage] device lost.");
	if (vkResetCommandPool(*vulkanDevice, mCommandPool, releaseResources ? VK_COMMAND_POOL_RESET_RELEASE_RESOURCES_BIT : 0) != VK_SUCCESS)
	{
		LOG("[Graphics::Comannd Pool] Error occured during reset!");
		return false;
	}
	return true;
}
