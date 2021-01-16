#pragma once
#include "Common.h"
#include "VulkanParameters.h"
#ifndef _DEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

namespace SingularityEngine::Vulkan
{

	static void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);

	class Instance
	{
	public:
		Instance();
		bool create(StartupParameters startupInfo);
		bool destroy();
		VkInstance& operator()(){ return mActualInstance; }
	private:
		const std::vector<const char*> validationLayers = {
		"VK_LAYER_KHRONOS_validation"
		};
		bool checkAvailableInstanceExtensions(std::vector<VkExtensionProperties>& availableExtensions);
		bool isExtensionSupported(const char* extensionName, const std::vector<VkExtensionProperties>& availableExtensions);
		bool checkValidationlayerSupport();
		VkInstance mActualInstance;
	};
}