#include "Precompiled.h"
#include "VulkanQueue.h"
#include "VulkanFunctions.h"

using namespace SingularityEngine::Graphics;

VulkanQueue::VulkanQueue()
{

}

void VulkanQueue::setQueueFamilyIndex(uint32_t index)
{
	mQueueFamilyIndex = index;
}

uint32_t VulkanQueue::getQueueFamilyIndex()
{
	ASSERT(mQueueFamilyIndex.has_value(), "Index was never set for queue, get will fail!");
	return *mQueueFamilyIndex;
}

void VulkanQueue::create(VkDevice& device)
{
	ASSERT(mQueueFamilyIndex.has_value(), "Index was never set for queue, initalization will fail!");
	vkGetDeviceQueue(device, *mQueueFamilyIndex, 0, &mQueue);
}

void VulkanQueue::destroy()
{
	mQueue = VK_NULL_HANDLE;
	mQueueFamilyIndex.reset();
}

bool VulkanQueue::hasQueueFamilyIndex()
{
	return mQueueFamilyIndex.has_value();
}

bool VulkanQueue::wait()
{
	if (vkQueueWaitIdle(mQueue) != VK_SUCCESS)
	{
		LOG("[GraphicsSystem] Failed to wait for queue!");
		return false;
	}
	return true;
}
