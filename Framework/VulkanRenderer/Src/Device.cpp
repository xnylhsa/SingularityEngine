#include "Precompiled.h"
#include "Device.h"
#include "VulkanFunctions.h"
using namespace SingularityEngine::Vulkan;

Device::Device(PresentationSurface& presentSurface) : mLogicalDevice(VK_NULL_HANDLE), mPhysicalDevice(VK_NULL_HANDLE), rPresentationSurface(presentSurface())
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
	uint32_t heighestScore = 0;
	//device needs to be chosen here?
	for (auto&& physicalDevice : availableDevices)
	{
		QueueFamilyIndicies deviceQueueFamilyIndicies;
		uint32_t score = ratePhysicalDeviceProperties(physicalDevice, startupInfo, deviceQueueFamilyIndicies);
		if(score > heighestScore)
		{
			mPhysicalDevice = physicalDevice;
			mTransferQueue.setQueueFamilyIndex(*deviceQueueFamilyIndicies.transferFamily);
			mPresentQueue.setQueueFamilyIndex(*deviceQueueFamilyIndicies.presentFamily);
			mGraphicsQueue.setQueueFamilyIndex(*deviceQueueFamilyIndicies.graphicsFamily);
			mComputeQueue.setQueueFamilyIndex(*deviceQueueFamilyIndicies.computeFamily);

		}
	}

	if (mPhysicalDevice == VK_NULL_HANDLE)
	{
		ASSERT(false, "Could not obtain physical device requested!");
		return false;
	}

	std::vector<QueueInfo> queueInfos;
	std::set<uint32_t> uniqueQueueFamilies = { mTransferQueue.getQueueFamilyIndex(), mComputeQueue.getQueueFamilyIndex(), mGraphicsQueue.getQueueFamilyIndex(), mPresentQueue.getQueueFamilyIndex() };

	for (uint32_t queuefamily : uniqueQueueFamilies)
	{
		queueInfos.push_back({ queuefamily, {1.0f} });
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

	VkResult result = vkCreateDevice(mPhysicalDevice, &deviceCreateInfo, nullptr, &mLogicalDevice);
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
		LOG("[Graphics System] Destroying graphics device");
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

uint32_t Device::ratePhysicalDeviceProperties(VkPhysicalDevice physicalDevice, StartupParameters& startupInfo, QueueFamilyIndicies& indicies)
{
	uint32_t score = 0;
	DeviceParameters& parameters = startupInfo.mDeviceParameters;
	std::vector<VkExtensionProperties> availableDeviceExtensions;
	if (!checkAvailableDevicesExtensions(physicalDevice, availableDeviceExtensions))
	{
		ASSERT(false, "Could not enumerate device extensions.");
		return 0;
	}

	for (auto& extension : parameters.mDesiredDeviceExtensions)
	{
		if (!isExtensionSupported(extension, availableDeviceExtensions))
		{
			ASSERT(false, "Extension named '%s' is not supported.", extension);
			return 0;
		}
	}

	VkPhysicalDeviceFeatures deviceFeatures;
	VkPhysicalDeviceProperties deviceProperties;

	vkGetPhysicalDeviceFeatures(physicalDevice, &deviceFeatures);
	vkGetPhysicalDeviceProperties(physicalDevice, &deviceProperties);

	uint32_t majorVersion = VK_VERSION_MAJOR(deviceProperties.apiVersion);
	if (majorVersion < 1)
	{
		ASSERT(false, "device %p does not contain neccesary version, or image dimensions", physicalDevice);
		return 0;
	}
	score += deviceProperties.limits.maxImageDimension2D;
	score += deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU ? 1000 : 0;

	if (!deviceFeatures.geometryShader)
	{
		return 0;
	}
	else
	{
		deviceFeatures = {};
		deviceFeatures.geometryShader = VK_TRUE;
		parameters.mDesiredFeatures = deviceFeatures;
	}

	if (!selectQueueMatchingDesiredCapabilities(physicalDevice, indicies))
	{
		return 0;
	}

	return score;
}

bool Device::selectQueueMatchingDesiredCapabilities(VkPhysicalDevice device, QueueFamilyIndicies& indicies)
{

	uint32_t queueFamiliesCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamiliesCount, nullptr);
	if (queueFamiliesCount == 0)
	{
		ASSERT(false, "Physical device %p does not have any queue families.", device);
		return 0;
	}
	std::vector<VkQueueFamilyProperties> queueFamilyProperties(queueFamiliesCount);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamiliesCount, queueFamilyProperties.data());
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamiliesCount, nullptr);
	if (queueFamiliesCount == 0)
	{
		ASSERT(false, "Physical device %p does not have any queue families.", device);
		return 0;
	}

	for (size_t i = 0; i < queueFamilyProperties.size(); i++)
	{
		VkBool32 presentSupport = VK_FALSE;
		vkGetPhysicalDeviceSurfaceSupportKHR(device, static_cast<uint32_t>(i), rPresentationSurface, &presentSupport);
		VkQueueFamilyProperties& properties = queueFamilyProperties[i];
		if (properties.queueFlags & VK_QUEUE_TRANSFER_BIT)
		{
			indicies.transferFamily = static_cast<uint32_t>(i);
		}
		if (properties.queueFlags & VK_QUEUE_COMPUTE_BIT)
		{
			indicies.computeFamily = static_cast<uint32_t>(i);
		}

		if (properties.queueFlags & VK_QUEUE_GRAPHICS_BIT && !indicies.graphicsFamily.has_value())
		{
			indicies.graphicsFamily = static_cast<uint32_t>(i);
		}
		if (presentSupport && !indicies.presentFamily.has_value())
		{
			indicies.presentFamily = static_cast<uint32_t>(i);
		}
		if (properties.queueFlags & VK_QUEUE_GRAPHICS_BIT && presentSupport)
		{
			indicies.graphicsFamily = static_cast<uint32_t>(i);
			indicies.presentFamily = static_cast<uint32_t>(i);
		}
	}
	if (!indicies.isComplete())
	{
		LOG("Could not find queues matching the desired parameters");
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

bool Device::createDeviceQueues()
{
	if (!mPresentQueue.hasQueueFamilyIndex() || !mTransferQueue.hasQueueFamilyIndex() || !mGraphicsQueue.hasQueueFamilyIndex() || !mComputeQueue.hasQueueFamilyIndex())
	{
		return false;
	}
	mPresentQueue.create(mLogicalDevice);
	mComputeQueue.create(mLogicalDevice);
	mGraphicsQueue.create(mLogicalDevice);
	mTransferQueue.create(mLogicalDevice);
	return true;
}

VkDevice& Device::getLogicalDevice()
{
	ASSERT(mLogicalDevice != VK_NULL_HANDLE, "{Graphics::Device] Logical Device has not been created yet!");
	return mLogicalDevice;
}

VkPhysicalDevice& Device::getPhysicalDevice()
{
	ASSERT(mPhysicalDevice != VK_NULL_HANDLE, "{Graphics::Device]Physical Device has not been chosen yet!");
	return mPhysicalDevice;
}

