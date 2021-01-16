#include "Precompiled.h"
#include "Instance.h"
#include "VulkanFunctions.h"

using namespace SingularityEngine::Vulkan;


Instance::Instance()
{
	mActualInstance = VK_NULL_HANDLE;
}

bool Instance::checkAvailableInstanceExtensions(std::vector<VkExtensionProperties>& availableExtensions)
{
	uint32_t extensionsCount = 0;
	VkResult result = VK_SUCCESS;
	result = vkEnumerateInstanceExtensionProperties(nullptr, &extensionsCount, nullptr);
	if ((result != VK_SUCCESS) || extensionsCount == 0)
	{
		ASSERT(false, "Could not get the number of Instance Extensions.");
		return false;
	}
	availableExtensions.resize(extensionsCount);
	result = vkEnumerateInstanceExtensionProperties(nullptr, &extensionsCount, &availableExtensions[0]);
	if ((result != VK_SUCCESS) || extensionsCount == 0)
	{
		ASSERT(false, "Could not enumerate Instance Extentsions.");
		return false;
	}
	return true;
}

bool Instance::create(StartupParameters startupInfo)
{
	if (enableValidationLayers && !checkValidationlayerSupport()) {
		ASSERT(false, "validation layers requested, but not available!");
		return false;
	}
	InstanceParameters& parameters = startupInfo.mInstanceParameters;

	std::vector<VkExtensionProperties> availableExtensions;
	if (!checkAvailableInstanceExtensions(availableExtensions))
	{
		return false;
	}

	for (auto& extension : parameters.mDesiredExtensions)
	{
		if (!isExtensionSupported(extension, availableExtensions))
		{
			ASSERT(false, "Extension named '%s' is not supported.", extension);
			return false;
		}
	}
	VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;
	if (enableValidationLayers)
	{
		populateDebugMessengerCreateInfo(debugCreateInfo);
	}

	VkApplicationInfo applicationInfo = {
		VK_STRUCTURE_TYPE_APPLICATION_INFO,
		nullptr,
		startupInfo.mAppName.c_str(),
		VK_MAKE_VERSION(startupInfo.mMajorVersion,startupInfo.mMinorVersion,startupInfo.mPatchVersion),
		startupInfo.mEngineName.c_str(),
		VK_MAKE_VERSION(startupInfo.mEngineMajorVersion,startupInfo.mEngineMinorVersion,startupInfo.mEnginePatchVersion),
		VK_API_VERSION_1_2
	};

	VkInstanceCreateInfo instanceCreateInfo = {
		VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
		enableValidationLayers ? &debugCreateInfo : nullptr,
		0,
		&applicationInfo,
		enableValidationLayers ? static_cast<uint32_t>(validationLayers.size()): 0,
		enableValidationLayers ? validationLayers.data() : nullptr,
		static_cast<uint32_t>(parameters.mDesiredExtensions.size()),
		parameters.mDesiredExtensions.size() > 0 ? &parameters.mDesiredExtensions[0] : nullptr
	};
	VkResult result = vkCreateInstance(&instanceCreateInfo, nullptr, &mActualInstance);
	if ((result != VK_SUCCESS) || mActualInstance == VK_NULL_HANDLE)
	{
		ASSERT(false, "Could not create Vulkan Instance.");
		return false;
	}
	return true;
}

bool Instance::destroy()
{
	LOG("[Graphics System] Destroying graphics instance");

	if (mActualInstance)
	{
		vkDestroyInstance(mActualInstance, nullptr);
		mActualInstance = VK_NULL_HANDLE;
	}
	return true;
}

bool Instance::isExtensionSupported(const char* extensionName, const std::vector<VkExtensionProperties>& availableExtensions)
{
	for (size_t i = 0; i < availableExtensions.size(); i++)
	{
		if (strcmp(availableExtensions[i].extensionName, extensionName) == 0)
		{
			return true;
		}
	}
	return false;
}

bool Instance::checkValidationlayerSupport()
{
	uint32_t layerCount;
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
	std::vector<VkLayerProperties> availableLayers(layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());
	for (const char* layerName : validationLayers) {
		bool layerFound = false;

		for (const auto& layerProperties : availableLayers) {
			if (strcmp(layerName, layerProperties.layerName) == 0) {
				layerFound = true;
				break;
			}
		}

		if (!layerFound) {
			return false;
		}
	}
	return true;
}

void SingularityEngine::Vulkan::populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo)
{
	createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
	createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
	createInfo.pfnUserCallback = debugCallback;
}
