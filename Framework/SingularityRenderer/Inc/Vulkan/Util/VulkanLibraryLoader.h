#pragma once
#include "Common.h"
#include "VulkanParameters.h"

namespace SingularityEngine::SERenderer
{
	class VulkanLibraryLoader
	{
	public:
		static bool init();
		static bool loadInstanceFuncs(VkInstance instance, const DesiredExtensions& extensions);
		static bool loadDeviceFuncs(VkDevice device, const DesiredExtensions& extensions);
		static bool shutdown();
	private:
		static bool loadExportedEntryPoints();
		static bool loadGlobalLevelEntryPoints();
		static bool loadLibrary();
		static bool loadInstanceLevelEntryPoints(VkInstance instance);
		static bool loadExtensionLevelEntryPoints(VkInstance instance, const DesiredExtensions& extensions);
		static bool loadDeviceLevelEntryPoints(VkDevice device);
		static bool loadDeviceLevelExtensionEntryPoints(VkDevice device, const DesiredExtensions& extensions);
		inline static SingularityEngine::Core::OS::LibraryHandle mVulkanLibrary;
	};
}
