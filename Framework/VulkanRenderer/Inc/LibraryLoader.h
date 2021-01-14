#pragma once
#include "Common.h"

namespace SingularityEngine::Vulkan
{
	class LibraryLoader
	{
	public:
		bool init();
		bool loadInstanceFuncs(VkInstance& instance);
		bool loadDeviceFuncs(VkDevice& device);
		bool shutdown();
	private:
		bool loadExportedEntryPoints();

		bool loadGlobalLevelEntryPoints();
		bool loadLibrary();
		bool loadInstanceLevelEntryPoints(VkInstance& instance);
		bool loadExtensionLevelEntryPoints(VkInstance& instance);
		bool loadDeviceLevelEntryPoints(VkDevice& device);
		bool loadDeviceLevelExtensionEntryPoints(VkDevice& device);
		SingularityEngine::Core::OS::LibraryHandle mVulkanLibrary;
	};
}
