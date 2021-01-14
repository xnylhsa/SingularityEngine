#include "Precompiled.h"
#include "GraphicsSystem.h"
#include "vulkanFunctions.h"
using namespace SingularityEngine;
using namespace Vulkan;

GraphicsSystem::GraphicsSystem()
{

}

GraphicsSystem::~GraphicsSystem()
{

}

bool GraphicsSystem::init(StartupParameters& startupInfo)
{
	if (!mVulkanLibrary.init())
	{
		return false;
	}
	if (!mVulkanInstance.create(startupInfo))
	{
		return false;
	}
	if (!mVulkanLibrary.loadInstanceFuncs(mVulkanInstance()))
	{
		return false;
	}
	if (!mVulkanDevice.create(startupInfo, mVulkanInstance))
	{
		return false;
	}
	if (!mVulkanLibrary.loadDeviceFuncs(mVulkanDevice()))
	{
		return false;
	}
	if (!getDeviceQueues(startupInfo))
	{
		return false;
	}

	return true;
}

bool GraphicsSystem::shutdown()
{
	return mVulkanDevice.destroy() && mVulkanInstance.destroy() && mVulkanLibrary.shutdown();
}

bool GraphicsSystem::getDeviceQueues(StartupParameters& startupInfo)
{
	std::vector<QueueParameters>& queueParameters = startupInfo.mQueueParameters;
	mQueues.clear();
	for (size_t i = 0; i < queueParameters.size(); i++)
	{
		mQueues.push_back(GraphicsQueueFamily(queueParameters[i].mQueueFamilyIndex));
		mQueues[i].init(mVulkanDevice, queueParameters[i].mQueuePriorities);
	}
	return true;
}




