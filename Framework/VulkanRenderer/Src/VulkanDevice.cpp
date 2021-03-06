#include "Precompiled.h"
#include "VulkanDevice.h"

#include "VulkanContext.h"
#include "VulkanFunctions.h"
#include "VulkanInstance.h"

using namespace SingularityEngine::Graphics;

VulkanDevice::VulkanDevice(StartupParameters& startupInfo) : mLogicalDevice(VK_NULL_HANDLE), mPhysicalDevice(VK_NULL_HANDLE)
{
	std::vector<VkPhysicalDevice> availableDevices;
	if (!checkAvailablePhysicalDevices(availableDevices))
	{
		ASSERT(false, "Error occured during physical device enumeration.");
	}
	uint32_t heighestScore = 0;
	//device needs to be chosen here?
	for (auto&& physicalDevice : availableDevices)
	{
		uint32_t score = ratePhysicalDeviceProperties(physicalDevice, startupInfo);
		if (score > heighestScore)
		{
			mPhysicalDevice = physicalDevice;
		}
	}
	if (mPhysicalDevice == VK_NULL_HANDLE)
	{
		ASSERT(false, "Could not obtain physical device requested!");
		return;
	}
	setPhysicalDeviceInfo();
	
	std::vector<QueueInfo> queueInfos;
	std::set<uint32_t> uniqueQueueFamilies = { mQueueFamilyIndicies.computeFamily.value(), mQueueFamilyIndicies.transferFamily.value(), mQueueFamilyIndicies.graphicsFamily.value() };

	for (uint32_t queuefamily : uniqueQueueFamilies)
	{
		queueInfos.push_back({ queuefamily, {0.0f} });
	}

	std::vector<VkDeviceQueueCreateInfo> queueCreateInfo;
	for (auto&& info : queueInfos)
	{
		queueCreateInfo.push_back({
			VkStructureType::VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,//stype
			nullptr,                                                    //pNext
			0,                                                          //flags
			info.queueFamilyIndex,                                           //queueFamilyIndex
			static_cast<uint32_t>(info.priorities.size()),              //queueCount
			!info.priorities.empty() ? &info.priorities[0] : nullptr  //pQueuePriorities
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
		static_cast<uint32_t>(startupInfo.mDeviceExtensions.size()),                           //enabledExtensionCount
		!startupInfo.mDeviceExtensions.empty() ? startupInfo.mDeviceExtensions.data() : nullptr,  //ppEnabledExtensionNames
		nullptr                                                                           //pEnabledFeatures
	};

	VkResult result = vkCreateDevice(mPhysicalDevice, &deviceCreateInfo, nullptr, &mLogicalDevice);
	if (result != VK_SUCCESS || mLogicalDevice == VK_NULL_HANDLE) {
		ASSERT(false, "Could not create logical device!");
		mPhysicalDevice = VK_NULL_HANDLE;
		mLogicalDevice = VK_NULL_HANDLE;
		return;
	}

}

VulkanDevice::~VulkanDevice()
{
	if (mLogicalDevice)
	{
		LOG("[Graphics System] Destroying graphics device");
		vkDestroyDevice(mLogicalDevice, nullptr);
		mLogicalDevice = VK_NULL_HANDLE;
	}
	return;
}

bool VulkanDevice::checkAvailablePhysicalDevices(std::vector<VkPhysicalDevice>& availableDevices)
{
	VulkanInstance* vulkanInstance = VulkanContext::GetVulkanInstance();
	uint32_t devicesCount = 0;
	VkResult result = VK_SUCCESS;
	result = vkEnumeratePhysicalDevices(*vulkanInstance, &devicesCount, nullptr);
	if ((result != VK_SUCCESS) || devicesCount == 0)
	{
		ASSERT(false, "Could not get the number of available physical devices!");
		return false;
	}
	availableDevices.resize(devicesCount);
	result = vkEnumeratePhysicalDevices(*vulkanInstance, &devicesCount, &availableDevices[0]);
	if ((result != VK_SUCCESS) || devicesCount == 0)
	{
		ASSERT(false, "Could Not Enumerate Physical Devices!");
		return false;
	}

	return true;
}

bool VulkanDevice::checkAvailableDevicesExtensions(VkPhysicalDevice physicalDevice, std::vector<VkExtensionProperties>& availableExtensions)
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

uint32_t VulkanDevice::ratePhysicalDeviceProperties(VkPhysicalDevice physicalDevice, StartupParameters& startupInfo)
{
	uint32_t score = 0;
	std::vector<VkExtensionProperties> availableDeviceExtensions;
	if (!checkAvailableDevicesExtensions(physicalDevice, availableDeviceExtensions))
	{
		ASSERT(false, "Could not enumerate device extensions.");
		return 0;
	}

	for (auto& extension : startupInfo.mDeviceExtensions)
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
		startupInfo.mDesiredDeviceFeatures = deviceFeatures;
	}
	return score;
}

bool VulkanDevice::selectQueueMatchingDesiredCapabilities(VkPhysicalDevice device)
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
		//VkBool32 presentSupport = VK_FALSE;
		VkQueueFamilyProperties& properties = queueFamilyProperties[i];
		if (properties.queueFlags & VK_QUEUE_TRANSFER_BIT)
		{
			mQueueFamilyIndicies.transferFamily = static_cast<uint32_t>(i);
		}
		if (properties.queueFlags & VK_QUEUE_COMPUTE_BIT)
		{
			mQueueFamilyIndicies.computeFamily = static_cast<uint32_t>(i);
		}
		if (properties.queueFlags & VK_QUEUE_GRAPHICS_BIT && !mQueueFamilyIndicies.graphicsFamily.has_value())
		{
			mQueueFamilyIndicies.graphicsFamily = static_cast<uint32_t>(i);
		}
	}
	if (!mQueueFamilyIndicies.isComplete())
	{
		LOG("Could not find queues matching the desired parameters");
		return false;
	}
	mGraphicsQueue.setQueueFamilyIndex(mQueueFamilyIndicies.graphicsFamily.value());
	return true;
}

bool VulkanDevice::isExtensionSupported(const char* extensionName, const std::vector<VkExtensionProperties>& availableExtensions)
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

bool VulkanDevice::createDeviceQueues()
{
	mCommandPool.create(mQueueFamilyIndicies.graphicsFamily.value());

	if (!mGraphicsQueue.hasQueueFamilyIndex())
	{
		return false;
	}
	mGraphicsQueue.create(mLogicalDevice);
	return true;
}

VulkanDevice::operator VkDevice()
{
	ASSERT(mLogicalDevice != VK_NULL_HANDLE, "{Graphics::Device] Logical Device has not been created yet!");
	return mLogicalDevice;
}

VulkanDevice::operator VkPhysicalDevice()
{
	ASSERT(mPhysicalDevice != VK_NULL_HANDLE, "[Graphics::Device]Physical Device has not been chosen yet!");
	return mPhysicalDevice;
}

uint32_t VulkanDevice::getMemoryTypeIndex(uint32_t typeBits, VkMemoryPropertyFlags properties)
{
	for (uint32_t i = 0; i < mMemoryProperties.memoryTypeCount; i++)
	{
		if ((typeBits & 1) == 1)
		{
			if ((mMemoryProperties.memoryTypes[i].propertyFlags & properties) == properties)
				return i;
		}
		typeBits >>= 1;
	}
	return UINT32_MAX;
}


bool VulkanDevice::setPhysicalDeviceInfo()
{
	vkGetPhysicalDeviceMemoryProperties(mPhysicalDevice, &mMemoryProperties);
	vkGetPhysicalDeviceFeatures(mPhysicalDevice, &mDeviceFeatures);
	vkGetPhysicalDeviceProperties(mPhysicalDevice, &mDeviceProperties);
	return selectQueueMatchingDesiredCapabilities(mPhysicalDevice);
}

VkFormat VulkanDevice::findFirstCompatibleDepthFormat(std::vector<VkFormat> depthFormats)
{
	for (auto&& format : depthFormats)
	{
		VkFormatProperties formatProps;
		vkGetPhysicalDeviceFormatProperties(mPhysicalDevice, format, &formatProps);
		if (formatProps.optimalTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT)
		{
			return format;
		}
	}
	return VK_FORMAT_MAX_ENUM;
}

bool VulkanDevice::wait()
{
	if (vkDeviceWaitIdle(mLogicalDevice) != VK_SUCCESS)
	{
		LOG("[GraphicsSystem] Failed to wait on device!");
		return false;
	}
	return true;
}

bool VulkanDevice::destroyDeviceQueues()
{
	mGraphicsQueue.destroy();
	return mCommandPool.destroy(); 
}
