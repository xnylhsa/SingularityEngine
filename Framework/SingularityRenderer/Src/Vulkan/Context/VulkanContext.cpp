#include "Precompiled.h"
#include "Vulkan/Context/VulkanContext.h"
#include "Vulkan/Util/vulkanFunctions.h"

#include "Vulkan/MemBuffer.h"
#include "Vulkan/Memory/VulkanMemoryObject.h"


namespace SingularityEngine::SERenderer
{
	VulkanContext::VulkanContext()
	{
	}

	VulkanContext::~VulkanContext()
	{
		if (sVulkanInstance)
		{
			teardown();
			LOG("[SERenderer::VulkanContext] teardown was never called!");
		}
	}

	bool VulkanContext::teardown()
	{

		mVulkanDevice->destroyDeviceQueues();
		mVulkanDevice.reset();
		if (enableValidationLayers) {
			vkDestroyDebugUtilsMessengerEXT(*sVulkanInstance, debugMessenger, nullptr);
		}
		sVulkanInstance.reset();
		VulkanLibraryLoader::shutdown();
		return true;
	}

	bool VulkanContext::setupDebugMessenger()
	{
		if (!enableValidationLayers)
		{
			return true;
		}

		VkDebugUtilsMessengerCreateInfoEXT createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
		createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
		createInfo.pfnUserCallback = debugCallback;
		createInfo.pUserData = nullptr; // Optional

		if (vkCreateDebugUtilsMessengerEXT(*sVulkanInstance, &createInfo, nullptr, &debugMessenger) != VK_SUCCESS)
		{
			LOG("Failed to load validation layers debug messenger!");
			return false;
		}

		return true;
	}

	VulkanInstance* VulkanContext::GetVulkanInstance()
	{
		ASSERT(sVulkanInstance->isValid(), "[Graphics::VulkanContext] Vulkan Instance is invalid!");
		return sVulkanInstance.get();
	}

	bool VulkanContext::buildCommandBuffer()
	{
		return true;
	}

	bool VulkanContext::initialize()
	{
		ASSERT(sVulkanInstance == nullptr, "[Graphics::GraphicsSystem] System already initialized!");

		DesiredExtensions desiredInstanceExtensions;
		fillDesiredInstanceExtensions(desiredInstanceExtensions);


		fillDesiredDeviceExtensions(mDesiredDeviceExtensions);

		LOG("[SERenderer::VulkanContext] Initalizing...");
		if (!VulkanLibraryLoader::init())
		{
			return false;
		}

		LOG("[SERenderer::VulkanContext] Creating Instance...");
		sVulkanInstance = std::make_unique<VulkanInstance>(desiredInstanceExtensions);
		if (!sVulkanInstance->isValid())
		{
			return false;
		}
		LOG("[SERenderer::VulkanContext] Instance Created...");

		if (!VulkanLibraryLoader::loadInstanceFuncs(*sVulkanInstance, desiredInstanceExtensions))
		{
			return false;
		}
		LOG("[SERenderer::VulkanContext] Initalized!");
		return true;
	}

	std::shared_ptr<VulkanDevice> VulkanContext::GetVulkanDevice()
	{
		return mVulkanDevice;
	}

	bool VulkanContext::isInitalized()
	{
		return mIsInitalized;
	}

	void VulkanContext::fillDesiredInstanceExtensions(DesiredExtensions& extensions)
	{
		extensions.push_back(VK_KHR_SURFACE_EXTENSION_NAME);
#if defined(_WIN32) || (_WIN64)
		extensions.push_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
#elif defined(VK_USE_PLATFORM_XCB_KHR)
		extensions.push_back(VK_KHR_XCB_SURFACE_EXTENSION_NAME);
#elif defined(VK_USE_PLATFORM_XLIB_KHR)
		extensions.push_back(VK_KHR_XLIB_SURFACE_EXTENSION_NAME);
#endif
		if (enableValidationLayers) {
			extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
		}
	}

	void VulkanContext::fillDesiredDeviceExtensions(DesiredExtensions& extensions)
	{
		extensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
	}

}
