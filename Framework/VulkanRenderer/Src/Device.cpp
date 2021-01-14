#include "Precompiled.h"
#include "Device.h"
#include "VulkanFunctions.h"

using namespace SingularityEngine::Vulkan;

Device::Device() : mLogicalDevice(VK_NULL_HANDLE), mPhysicalDevices(0)
{
}

Device::~Device()
{

}

bool Device::create(StartupParameters& startupInfo, Instance& instance)
{
	DeviceParameters& parameters = startupInfo.mDeviceParameters;
	std::vector<VkPhysicalDevice> availableDevices;
	if (!checkAvailablePhysicalDevices(availableDevices, instance))
	{
		ASSERT(false, "Error occured during physical device enumeration.");
		return false;
	}

	//device needs to be chosen here?
	for (auto&& physicalDevice : availableDevices)
	{
		if (checkPhysicalDeviceProperties(physicalDevice, startupInfo))
		{
			mPhysicalDevices.push_back(physicalDevice);
		}
	}
	if (mPhysicalDevices.size() == 0)
	{
		ASSERT(false, "Could not obtain physical device requested!");
		return false;

	}

	std::vector<QueueInfo> queueInfos;
	for (size_t i = 0; i < startupInfo.mQueueParameters.size(); i++)
	{
		QueueParameters& qParameters = startupInfo.mQueueParameters[i];
		if (qParameters.mQueueFamilyIndex != UINT32_MAX)
		{
			queueInfos.push_back({ qParameters.mQueueFamilyIndex, qParameters.mQueuePriorities });
		}
	}

	std::vector<VkDeviceQueueCreateInfo> queueCreateInfo;
	for (auto&& info : queueInfos)
	{
		queueCreateInfo.push_back({
			VkStructureType::VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,//stype
			nullptr,                                                    //pNext
			0,                                                          //flags
			info.FamilyIndex,                                           //queueFamilyIndex
			static_cast<uint32_t>(info.Priorities.size()),              //queueCount
			info.Priorities.size() > 0 ? &info.Priorities[0] : nullptr  //pQueuePriorities
			});
	}

	VkDeviceCreateInfo deviceCreateInfo = {
		VkStructureType::VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,                            //stype
		nullptr,                                                                          //pNext
		0,                                                                                //Flags
		static_cast<uint32_t>(queueCreateInfo.size()),                                    //queueCreateInfoCount
		queueCreateInfo.data(),                                                           //pQueueCreateInfos
		0,                                                                                //enabledLayerCount
		nullptr,                                                                          //ppEnabledLayerNames
		static_cast<uint32_t>(parameters.mDesiredDeviceExtensions.size()),                           //enabledExtensionCount
		parameters.mDesiredDeviceExtensions.size() > 0 ? parameters.mDesiredDeviceExtensions.data() : nullptr,  //ppEnabledExtensionNames
		nullptr                                                                           //pEnabledFeatures
	};

	VkResult result = vkCreateDevice(mPhysicalDevices[0], &deviceCreateInfo, nullptr, &mLogicalDevice);
	if (result != VK_SUCCESS || mLogicalDevice == VK_NULL_HANDLE) {
		ASSERT(false, "Could not create logical device!");
		return false;
	}
	return true;
}

bool Device::destroy()
{
	if (mLogicalDevice)
	{
		vkDestroyDevice(mLogicalDevice, nullptr);
		mLogicalDevice = VK_NULL_HANDLE;
	}
	return true;
}

bool Device::checkAvailablePhysicalDevices(std::vector<VkPhysicalDevice>& availableDevices, Instance& instance)
{
	uint32_t devicesCount = 0;
	VkResult result = VK_SUCCESS;
	result = vkEnumeratePhysicalDevices(instance(), &devicesCount, nullptr);
	if ((result != VK_SUCCESS) || devicesCount == 0)
	{
		ASSERT(false, "Could not get the number of available physical devices!");
		return false;
	}
	availableDevices.resize(devicesCount);
	result = vkEnumeratePhysicalDevices(instance(), &devicesCount, &availableDevices[0]);
	if ((result != VK_SUCCESS) || devicesCount == 0)
	{
		ASSERT(false, "Could Not Enumerate Physical Devices!");
		return false;
	}

	return true;
}

bool Device::checkAvailableDevicesExtensions(VkPhysicalDevice physicalDevice, std::vector<VkExtensionProperties>& availableExtensions)
{
	uint32_t extensionsCount = 0;
	VkResult result = VK_SUCCESS;
	result = vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionsCount, nullptr);
	if ((result != VK_SUCCESS) || extensionsCount == 0)
	{
		ASSERT(false, "Could not get the number of available device extensions!");
		return false;
	}
	availableExtensions.resize(extensionsCount);
	result = vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionsCount, &availableExtensions[0]);
	if ((result != VK_SUCCESS) || extensionsCount == 0)
	{
		ASSERT(false, "Could Not Enumerate Device Extensions!");
		return false;
	}

	return true;
}

bool Device::checkPhysicalDeviceProperties(VkPhysicalDevice physicalDevice, StartupParameters& startupInfo)
{
	DeviceParameters& parameters = startupInfo.mDeviceParameters;

	std::vector<VkExtensionProperties> availableDeviceExtensions;
	if (!checkAvailableDevicesExtensions(physicalDevice, availableDeviceExtensions))
	{
		ASSERT(false, "Could not enumerate device extensions.");
		return false;
	}


	for (auto& extension : parameters.mDesiredDeviceExtensions)
	{
		if (!isExtensionSupported(extension, availableDeviceExtensions))
		{
			ASSERT(false, "Extension named '%s' is not supported.", extension);
			return false;
		}
	}

	VkPhysicalDeviceFeatures deviceFeatures;
	VkPhysicalDeviceProperties deviceProperties;

	vkGetPhysicalDeviceFeatures(physicalDevice, &deviceFeatures);
	vkGetPhysicalDeviceProperties(physicalDevice, &deviceProperties);

	uint32_t majorVersion = VK_VERSION_MAJOR(deviceProperties.apiVersion);
	if (majorVersion < 1 || deviceProperties.limits.maxImageDimension2D < 4096)
	{
		ASSERT(false, "device %p does not contain neccesary version, or image dimensions", physicalDevice);
		return false;
	}

	if (!deviceFeatures.geometryShader)
	{
		return false;
	}
	else
	{
		deviceFeatures = {};
		deviceFeatures.geometryShader = VK_TRUE;
		parameters.mDesiredFeatures = deviceFeatures;
	}

	uint32_t queueFamiliesCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamiliesCount, nullptr);
	if (queueFamiliesCount == 0)
	{
		ASSERT(false, "Physical device %p does not have any queue families.", physicalDevice);
		return false;
	}
	std::vector<VkQueueFamilyProperties> queueFamilyProperties(queueFamiliesCount);
	std::vector<VkBool32> queuePresentSupport(queueFamiliesCount);
	vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamiliesCount, queueFamilyProperties.data());
	vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamiliesCount, nullptr);
	if (queueFamiliesCount == 0)
	{
		ASSERT(false, "Physical device %p does not have any queue families.", physicalDevice);
		return false;
	}

	for (size_t i = 0; i < startupInfo.mQueueParameters.size(); i++)
	{
		QueueParameters& qParameters = startupInfo.mQueueParameters[i];
		if (!selectQueueMatchingDesiredCapabilities(qParameters.desiredCapabilites, qParameters.mQueueFamilyIndex, queueFamilyProperties))
		{
			return false;
		}
	}

	return true;
}

bool Device::selectQueueMatchingDesiredCapabilities(VkQueueFlags desiredCapabilites, uint32_t& queueIndex, std::vector<VkQueueFamilyProperties>& queueFamilyProperties)
{
	for (size_t i = 0; i < queueFamilyProperties.size(); i++)
	{
		VkQueueFamilyProperties& properties = queueFamilyProperties[i];
		if (properties.queueCount > 0 && (properties.queueFlags & desiredCapabilites) == 0)
		{
			queueIndex = static_cast<uint32_t>(i);
		}
	}
	if (queueIndex == UINT32_MAX)
	{
		ASSERT(false, "Could not find a queue matching the desired parameters");
		return false;
	}
	return true;
}

bool Device::isExtensionSupported(const char* extensionName, const std::vector<VkExtensionProperties>& availableExtensions)
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
