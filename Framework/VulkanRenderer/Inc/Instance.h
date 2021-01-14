#pragma once
#include "Common.h"
#include "VulkanParameters.h"


namespace SingularityEngine::Vulkan
{
	class Instance
	{
	public:
		Instance();
		bool create(StartupParameters startupInfo);
		bool destroy();
		VkInstance& operator()(){ return mActualInstance; }
	private:
		bool checkAvailableInstanceExtensions(std::vector<VkExtensionProperties>& availableExtensions);
		bool isExtensionSupported(const char* extensionName, const std::vector<VkExtensionProperties>& availableExtensions);

		VkInstance mActualInstance;
	};
}