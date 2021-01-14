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
		nullptr,
		0,
		&applicationInfo,
		0,
		nullptr,
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