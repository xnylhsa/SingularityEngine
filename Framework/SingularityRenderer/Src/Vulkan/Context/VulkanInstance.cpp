#include "Precompiled.h"
#include "Vulkan/Context/VulkanInstance.h"
#include "Vulkan/Util/VulkanFunctions.h"

using namespace SingularityEngine::SERenderer;


VulkanInstance::VulkanInstance(const DesiredExtensions& desiredExtensions)
{
	mActualInstance = VK_NULL_HANDLE;
	if (enableValidationLayers && !checkValidationlayerSupport()) {
		ASSERT(false, "validation layers requested, but not available!");
		return;
	}

	std::vector<VkExtensionProperties> availableExtensions;
	if (!checkAvailableInstanceExtensions(availableExtensions))
	{
		return;
	}

	for (auto& extension : desiredExtensions)
	{
		if (!isExtensionSupported(extension, availableExtensions))
		{
			ASSERT(false, "Extension named '%s' is not supported.", extension);
			return;
		}
	}
	VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;
	if (enableValidationLayers)
	{
		populateDebugMessengerCreateInfo(debugCreateInfo);
	}
	Core::AppVer appVersion = Core::Application::get()->getVersion();
	std::string appName = Core::Application::get()->getWindow()->getTitle();
	VkApplicationInfo applicationInfo = {
		VK_STRUCTURE_TYPE_APPLICATION_INFO,
		nullptr,
		appName.c_str(),
		VK_MAKE_VERSION(appVersion.majorVersion,appVersion.minorVersion,appVersion.patchVersion),
		SINGULARITY_ENGINE_NAME,
		VK_MAKE_VERSION(SINGULARITY_ENGINE_MAJOR_VERSION, SINGULARITY_ENGINE_MINOR_VERSION, SINGULARITY_ENGINE_PATCH_VERSION),
		VK_API_VERSION_1_2
	};

	VkInstanceCreateInfo instanceCreateInfo = {
		VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
		enableValidationLayers ? &debugCreateInfo : nullptr,
		0,
		&applicationInfo,
		enableValidationLayers ? static_cast<uint32_t>(validationLayers.size()) : 0,
		enableValidationLayers ? validationLayers.data() : nullptr,
		static_cast<uint32_t>(desiredExtensions.size()),
		!desiredExtensions.empty() ? &desiredExtensions[0] : nullptr
	};
	VkResult result = vkCreateInstance(&instanceCreateInfo, nullptr, &mActualInstance);
	if ((result != VK_SUCCESS) || mActualInstance == VK_NULL_HANDLE)
	{
		ASSERT(false, "Could not create Vulkan Instance.");
		return;
	}
}

bool VulkanInstance::checkAvailableInstanceExtensions(std::vector<VkExtensionProperties>& availableExtensions)
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

VulkanInstance::~VulkanInstance()
{
	LOG("[Graphics System] Destroying graphics instance");
	if (mActualInstance)
	{
		vkDestroyInstance(mActualInstance, nullptr);
		mActualInstance = VK_NULL_HANDLE;
	}
	return;
}

bool VulkanInstance::isExtensionSupported(const char* extensionName, const std::vector<VkExtensionProperties>& availableExtensions)
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

bool VulkanInstance::checkValidationlayerSupport()
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

void SingularityEngine::SERenderer::populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo)
{
	createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
	createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
	createInfo.pfnUserCallback = debugCallback;
}
