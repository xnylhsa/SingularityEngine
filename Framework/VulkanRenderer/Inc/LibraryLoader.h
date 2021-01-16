#pragma once
#include "Common.h"
#include "VulkanParameters.h"

namespace SingularityEngine::Vulkan
{
	class LibraryLoader
	{
	public:
		bool init();
		bool loadInstanceFuncs(VkInstance& instance, StartupParameters startupInfo);
		bool loadDeviceFuncs(VkDevice& device, StartupParameters startupInfo);
		bool shutdown();
	private:
		bool loadExportedEntryPoints();

		bool loadGlobalLevelEntryPoints();
		bool loadLibrary();
		bool loadInstanceLevelEntryPoints(VkInstance& instance);
		bool loadExtensionLevelEntryPoints(VkInstance& instance, StartupParameters startupInfo);
		bool loadDeviceLevelEntryPoints(VkDevice& device);
		bool loadDeviceLevelExtensionEntryPoints(VkDevice& device, StartupParameters startupInfo);
		SingularityEngine::Core::OS::LibraryHandle mVulkanLibrary;
	};
}
