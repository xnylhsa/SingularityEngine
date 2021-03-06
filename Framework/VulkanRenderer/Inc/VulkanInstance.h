#pragma once
#include "Common.h"
#include "VulkanParameters.h"
#ifndef _DEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

namespace SingularityEngine::Graphics
{

	static void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);

	class VulkanInstance
	{
	public:
		VulkanInstance(StartupParameters startupInfo);
		~VulkanInstance();
		operator VkInstance() { return mActualInstance; }
		bool isValid() { return mActualInstance != VK_NULL_HANDLE; }
	private:
		const std::vector<const char*> validationLayers = {
		"VK_LAYER_KHRONOS_validation"
		};
		bool checkAvailableInstanceExtensions(std::vector<VkExtensionProperties>& availableExtensions);
		bool isExtensionSupported(const char* extensionName, const std::vector<VkExtensionProperties>& availableExtensions);
		bool checkValidationlayerSupport();
		VkInstance mActualInstance = VK_NULL_HANDLE;
	};
}