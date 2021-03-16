#include "Precompiled.h"
#include "Vulkan/Device/VulkanDevice.h"

#include "Vulkan/Context/VulkanContext.h"
#include "Vulkan/Util/VulkanFunctions.h"
#include "Vulkan/Context/VulkanInstance.h"
#include "vulkan/Memory/VulkanMemoryAllocator.h"
#include "vulkan/DescriptorSets/VulkanDescriptorSet.h"
#include "RendererAPI/Renderer.h"
using namespace SingularityEngine::SERenderer;

VulkanDevice::VulkanDevice() : mLogicalDevice(VK_NULL_HANDLE), mPhysicalDevice(VK_NULL_HANDLE)
{


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

uint32_t VulkanDevice::ratePhysicalDeviceProperties(VkPhysicalDevice physicalDevice, DesiredExtensions& desiredExtensions)
{
	uint32_t score = 0;
	std::vector<VkExtensionProperties> availableDeviceExtensions;
	if (!checkAvailableDevicesExtensions(physicalDevice, availableDeviceExtensions))
	{
		ASSERT(false, "Could not enumerate device extensions.");
		return 0;
	}

	for (auto& extension : desiredExtensions)
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

	deviceFeatures = {};
	deviceFeatures.geometryShader = VK_TRUE;
	mDeviceFeatures = deviceFeatures;
	return score;
}

bool VulkanDevice::selectQueueMatchingDesiredCapabilities(VkPhysicalDevice device)
{

	uint32_t queueFamiliesCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamiliesCount, nullptr);
	if (queueFamiliesCount == 0)
	{
		ASSERT(false, "Physical device %p does not have any queue families.", device);
		return false;
	}
	std::vector<VkQueueFamilyProperties> queueFamilyProperties(queueFamiliesCount);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamiliesCount, queueFamilyProperties.data());
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamiliesCount, nullptr);
	if (queueFamiliesCount == 0)
	{
		ASSERT(false, "Physical device %p does not have any queue families.", device);
		return false;
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
	mCommandPool.create(std::dynamic_pointer_cast<VulkanDevice>(shared_from_this()), mQueueFamilyIndicies.graphicsFamily.value());

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

bool VulkanDevice::waitUntilIdle()
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

bool VulkanDevice::initialize()
{
	std::vector<VkPhysicalDevice> availableDevices;
	if (!checkAvailablePhysicalDevices(availableDevices))
	{
		ASSERT(false, "Error occured during physical device enumeration.");
	}
	uint32_t heighestScore = 0;
	//device needs to be chosen here?
	auto* context = dynamic_cast<VulkanContext*>(Renderer::Get()->getRenderContext());
	DesiredExtensions& desiredExtensions = context->getDesiredDeviceExtensions();
	for (auto&& physicalDevice : availableDevices)
	{
		uint32_t score = ratePhysicalDeviceProperties(physicalDevice, desiredExtensions);
		if (score > heighestScore)
		{
			mPhysicalDevice = physicalDevice;
		}
	}
	if (mPhysicalDevice == VK_NULL_HANDLE)
	{
		ASSERT(false, "Could not obtain physical device requested!");
		return false;
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
	// Get the extension feature
	VkPhysicalDeviceFeatures2KHR physical_device_features{ VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2_KHR };
	VkPhysicalDeviceDescriptorIndexingFeatures features{ VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DESCRIPTOR_INDEXING_FEATURES };
	physical_device_features.pNext = &features;
	vkGetPhysicalDeviceFeatures2(mPhysicalDevice, &physical_device_features);

	features.descriptorBindingSampledImageUpdateAfterBind = VK_TRUE;
	features.descriptorBindingPartiallyBound = VK_TRUE;
	features.descriptorBindingUpdateUnusedWhilePending = VK_TRUE;
	features.descriptorBindingVariableDescriptorCount = VK_TRUE;
	features.runtimeDescriptorArray = VK_TRUE;

	VkDeviceCreateInfo deviceCreateInfo = {
		VkStructureType::VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,                            //stype
		&features,                                                                          //pNext
		0,                                                                                //Flags
		static_cast<uint32_t>(queueCreateInfo.size()),                                    //queueCreateInfoCount
		queueCreateInfo.data(),                                                           //pQueueCreateInfos
		0,                                                                                //enabledLayerCount
		nullptr,                                                                          //ppEnabledLayerNames
		static_cast<uint32_t>(desiredExtensions.size()),                           //enabledExtensionCount
		!desiredExtensions.empty() ? desiredExtensions.data() : nullptr,  //ppEnabledExtensionNames
		nullptr                                                                           //pEnabledFeatures
	};

	VkResult result = vkCreateDevice(mPhysicalDevice, &deviceCreateInfo, nullptr, &mLogicalDevice);
	if (result != VK_SUCCESS || mLogicalDevice == VK_NULL_HANDLE) {
		ASSERT(false, "Could not create logical device!");
		mPhysicalDevice = VK_NULL_HANDLE;
		mLogicalDevice = VK_NULL_HANDLE;
		return false;
	}

	if (!VulkanLibraryLoader::loadDeviceFuncs(mLogicalDevice, desiredExtensions))
	{
		return false;
	}
	if (!context->setupDebugMessenger())
	{
		return false;
	}

	createDeviceQueues();

	//VmaVulkanFunctions functions;
	//functions.vkAllocateMemory = vkAllocateMemory;
	//functions.vkBindBufferMemory = vkBindBufferMemory;
	//functions.vkBindBufferMemory2KHR = vkBindBufferMemory2KHR;
	//functions.vkBindImageMemory = vkBindImageMemory;
	//functions.vkBindImageMemory2KHR = vkBindImageMemory2KHR;
	//functions.vkCmdCopyBuffer = vkCmdCopyBuffer;
	//functions.vkCreateBuffer = vkCreateBuffer;
	//functions.vkCreateImage = vkCreateImage;
	//functions.vkDestroyBuffer = vkDestroyBuffer;
	//functions.vkDestroyImage= vkDestroyImage;
	//functions.vkFlushMappedMemoryRanges = vkFlushMappedMemoryRanges;
	//functions.vkFreeMemory = vkFreeMemory;
	//functions.vkGetBufferMemoryRequirements = vkGetBufferMemoryRequirements;
	//functions.vkGetBufferMemoryRequirements2KHR = vkGetBufferMemoryRequirements2KHR;
	//functions.vkGetImageMemoryRequirements = vkGetImageMemoryRequirements;
	//functions.vkGetImageMemoryRequirements2KHR = vkGetImageMemoryRequirements2KHR;
	//functions.vkGetPhysicalDeviceMemoryProperties = vkGetPhysicalDeviceMemoryProperties;
	//functions.vkGetPhysicalDeviceMemoryProperties2KHR = vkGetPhysicalDeviceMemoryProperties2KHR;
	//functions.vkGetPhysicalDeviceProperties = vkGetPhysicalDeviceProperties;
	//functions.vkInvalidateMappedMemoryRanges = vkInvalidateMappedMemoryRanges;
	//functions.vkMapMemory = vkMapMemory;
	//functions.vkUnmapMemory = vkUnmapMemory;



	VmaAllocatorCreateInfo allocatorInfo = {};
	allocatorInfo.vulkanApiVersion = VK_API_VERSION_1_2;
	allocatorInfo.physicalDevice = mPhysicalDevice;
	allocatorInfo.device = mLogicalDevice;
	allocatorInfo.instance = *context->GetVulkanInstance();
	//allocatorInfo.pVulkanFunctions = &functions;

	vmaCreateAllocator(&allocatorInfo, &mVMAAllocator);

	mAllocator = std::make_unique<VulkanMemoryAllocator>(std::dynamic_pointer_cast<VulkanDevice>(shared_from_this()), "Default");
	createStockSamplers();
	initBindless();

	return true;
}

void VulkanDevice::teardown()
{
	mBindlesDescripterSetAllocatorInt.reset();
	mBindlesDescripterSetAllocatorFloat.reset();
	destroyStockSamplers();
	mAllocator.reset();
	vmaDestroyAllocator(mVMAAllocator);
	destroyDeviceQueues();
	if (mLogicalDevice)
	{
		LOG("[SERenderer::VulkanDevice] Destroying graphics device");
		vkDestroyDevice(mLogicalDevice, nullptr);
		mLogicalDevice = VK_NULL_HANDLE;
	}
}

VkCommandBuffer VulkanDevice::getCommandBuffer(bool begin, bool compute /*= false*/)
{
	UNREFERENCED_PARAMETER(compute);
	VkCommandBuffer cmd =  mCommandPool.requestCommandBuffer();
	//VkCommandBuffer cmd = compute ? mComputeCommandPool.requestCommandBuffer() : mCommandPool.requestCommandBuffer();

	if (begin)
	{
		VkCommandBufferBeginInfo cmdBufferBeginInfo{};
		cmdBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		vkBeginCommandBuffer(cmd, &cmdBufferBeginInfo);
	}
	return cmd;
}

void VulkanDevice::submit(VkCommandBuffer commandBuffer)
{
	submit(commandBuffer, mGraphicsQueue);
}

void VulkanDevice::submit(VkCommandBuffer commandBuffer, VkQueue queue)
{
	const uint64_t DEFAULT_FENCE_TIMEOUT = 100000000000;

	ASSERT(commandBuffer != VK_NULL_HANDLE, "[SERenderer::Device] invalid command Buffer");

	ASSERT(vkEndCommandBuffer(commandBuffer) == VK_SUCCESS, "[SERenderer::Device] failed to end command buffer");

	VkSubmitInfo submitInfo = {};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffer;

	// Create fence to ensure that the command buffer has finished executing
	VkFenceCreateInfo fenceCreateInfo = {};
	fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceCreateInfo.flags = 0;
	VkFence fence;
	ASSERT(vkCreateFence(mLogicalDevice, &fenceCreateInfo, nullptr, &fence) == VK_SUCCESS, "[SERenderer::Device] failed to create fence.");

	// Submit to the queue
	ASSERT(vkQueueSubmit(queue, 1, &submitInfo, fence) == VK_SUCCESS, "[SERenderer::Device] Failed to submit queue");
	// Wait for the fence to signal that command buffer has finished executing
	ASSERT(vkWaitForFences(mLogicalDevice, 1, &fence, VK_TRUE, DEFAULT_FENCE_TIMEOUT) == VK_SUCCESS,  "[SERenderer::Device] failed to wait for fence.");

	vkDestroyFence(mLogicalDevice, fence, nullptr);
	vkFreeCommandBuffers(mLogicalDevice, mCommandPool, 1, &commandBuffer);
}

VkImage VulkanDevice::createImage()
{
	return VK_NULL_HANDLE;
}

std::shared_ptr<VulkanSampler> VulkanDevice::getStockSampler(SamplerTypes sampler)
{
	return mStockSamplers[static_cast<size_t>(sampler)];
}

void VulkanDevice::createStockSamplers()
{
	SamplerCreateInfo info = {};
	info.maxLOD = VK_LOD_CLAMP_NONE;
	info.maxAnisotropy = 1.0f;
	for (unsigned i = 0; i < static_cast<unsigned>(SamplerTypes::Count); i++)
	{
		auto mode = static_cast<SamplerTypes>(i);

		switch (mode)
		{
		case SamplerTypes::NearestShadow:
		case SamplerTypes::LinearShadow:
			info.compareEnable = true;
			info.compareOp = VK_COMPARE_OP_LESS_OR_EQUAL;
			break;

		default:
			info.compareEnable = false;
			break;
		}

		switch (mode)
		{
		case SamplerTypes::TrilinearClamp:
		case SamplerTypes::TrilinearWrap:
			info.mipMapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
			break;

		default:
			info.mipMapMode = VK_SAMPLER_MIPMAP_MODE_NEAREST;
			break;
		}

		switch (mode)
		{
		case SamplerTypes::LinearClamp:
		case SamplerTypes::LinearWrap:
		case SamplerTypes::TrilinearClamp:
		case SamplerTypes::TrilinearWrap:
		case SamplerTypes::LinearShadow:
			info.magFilter = VK_FILTER_LINEAR;
			info.minFilter = VK_FILTER_LINEAR;
			break;

		default:
			info.magFilter = VK_FILTER_NEAREST;
			info.minFilter = VK_FILTER_NEAREST;
			break;
		}

		switch (mode)
		{
		default:
		case SamplerTypes::LinearWrap:
		case SamplerTypes::NearestWrap:
		case SamplerTypes::TrilinearWrap:
			info.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
			info.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
			info.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
			break;

		case SamplerTypes::LinearClamp:
		case SamplerTypes::NearestClamp:
		case SamplerTypes::TrilinearClamp:
		case SamplerTypes::NearestShadow:
		case SamplerTypes::LinearShadow:
			info.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
			info.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
			info.addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
			break;
		}

		mStockSamplers[i] = std::make_shared<VulkanSampler>(info);
	}
}

void VulkanDevice::destroyStockSamplers()
{
	for (auto&& sampler : mStockSamplers)
	{
		sampler.reset();
		sampler = nullptr;
	}
}

void VulkanDevice::initBindless()
{
	DescriptorSetLayout layout;

	layout.bindingInfos[0].size = VULKAN_UNBOUND_ARRAY;
	layout.bindingInfos[0].type = BindingType::SeperateImage;
	for (unsigned i = 1; i < VULKAN_NUM_BINDINGS; i++)
	{
		layout.bindingInfos[i].size = 1;
		layout.bindingInfos[i].type = BindingType::SeperateImage;
	}
	uint32_t stagesForSets[VULKAN_NUM_BINDINGS] = { VK_SHADER_STAGE_ALL };
	mBindlesDescripterSetAllocatorInt = std::make_unique<VulkanDescriptorSetAllocator>(layout, stagesForSets);
	for (unsigned i = 0; i < VULKAN_NUM_BINDINGS; i++)
	{
		layout.bindingInfos[i].isFloatingPoint = true;
	}
	mBindlesDescripterSetAllocatorFloat = std::make_unique<VulkanDescriptorSetAllocator>(layout, stagesForSets);
}
