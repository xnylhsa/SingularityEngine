#include "Precompiled.h"
#include "GraphicsQueue.h"
#include "VulkanFunctions.h"

using namespace SingularityEngine::Vulkan;

GraphicsQueue::GraphicsQueue(uint32_t index) : mIndex(index)
{

}

void GraphicsQueue::init(Device& device, uint32_t familyIndex)
{
	ASSERT(mIndex != UINT32_MAX, "Index was never set for queue, initalization will fail!");
	vkGetDeviceQueue(device(), familyIndex, mIndex, &mQueue);
}

void GraphicsQueue::shutdown()
{
	mIndex = UINT32_MAX;
}

GraphicsQueueFamily::GraphicsQueueFamily(uint32_t familyIndex) : mQueueFamilyIndex(familyIndex)
{

}

void GraphicsQueueFamily::init(Device& device, std::vector<float> priorities)
{
	mQueues.clear();
	for (size_t i = 0; i < priorities.size(); i++)
	{
		mQueues.push_back(GraphicsQueue(static_cast<uint32_t>(i)));
		mQueues[i].init(device, mQueueFamilyIndex);
	}
}

void GraphicsQueueFamily::shutdown()
{
	mQueues.clear();
	mQueueFamilyIndex = UINT32_MAX;
}
