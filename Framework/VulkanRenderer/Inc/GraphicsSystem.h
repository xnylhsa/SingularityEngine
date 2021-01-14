#pragma once
#include "Common.h"
#include "LibraryLoader.h"
#include "VulkanParameters.h"
#include "Instance.h"
#include "Device.h"
#include "GraphicsQueue.h"
namespace SingularityEngine::Vulkan
{
	class GraphicsSystem
	{
	public:
		GraphicsSystem();
		~GraphicsSystem();
		bool init(StartupParameters& startupInfo);
		bool shutdown();

	private:

		bool getDeviceQueues(StartupParameters& startupInfo);
		std::vector<GraphicsQueueFamily> mQueues;

		Device mVulkanDevice;
		Instance mVulkanInstance;
		LibraryLoader mVulkanLibrary;
	};
}