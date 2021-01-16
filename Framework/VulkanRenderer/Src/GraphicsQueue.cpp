#include "Precompiled.h"
#include "GraphicsQueue.h"
#include "VulkanFunctions.h"

using namespace SingularityEngine::Vulkan;

GraphicsQueueFamily::GraphicsQueueFamily()
{

}

void GraphicsQueueFamily::setQueueFamilyIndex(uint32_t index)
{
	mQueueFamilyIndex = index;
}

uint32_t GraphicsQueueFamily::getQueueFamilyIndex()
{
	ASSERT(mQueueFamilyIndex.has_value(), "Index was never set for queue, get will fail!");
	return *mQueueFamilyIndex;
}

void GraphicsQueueFamily::create(VkDevice& device)
{
	ASSERT(mQueueFamilyIndex.has_value(), "Index was never set for queue, initalization will fail!");
	vkGetDeviceQueue(device, *mQueueFamilyIndex, 0, &mQueue);
}

void GraphicsQueueFamily::destroy()
{
	mQueue = VK_NULL_HANDLE;
	mQueueFamilyIndex.reset();
}

bool GraphicsQueueFamily::hasQueueFamilyIndex()
{
	return mQueueFamilyIndex.has_value();
}
