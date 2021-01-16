#include "Precompiled.h"
#include "SwapChain.h"
#include "VulkanFunctions.h"

using namespace SingularityEngine::Vulkan;

bool SwapChain::checkAvailablePresentationModes(std::vector<VkPresentModeKHR>& presentationModes)
{
	uint32_t modeCount = 0;
	VkResult result = vkGetPhysicalDeviceSurfacePresentModesKHR(rPhysicalDevice, rPresentationSurface, &modeCount, nullptr);
	if ((result != VK_SUCCESS) || modeCount == 0)
	{
		ASSERT(false, "[Graphics::Swapchain] Could not get the number of available Presentation modes!");
		return false;
	}
	presentationModes.resize(modeCount);
	result = vkGetPhysicalDeviceSurfacePresentModesKHR(rPhysicalDevice, rPresentationSurface, &modeCount, &presentationModes[0]);
	if ((result != VK_SUCCESS) || modeCount == 0)
	{
		ASSERT(false, "[Graphics::Swapchain] Could Not Enumerate Presentation modes!");
		return false;
	}

	return true;
}

bool SwapChain::create(uint32_t w, uint32_t h)
{
	ASSERT(rLogicalDevice != VK_NULL_HANDLE, "[Graphics::Swapchain] logical device handle is not set!");
	ASSERT(rPhysicalDevice != VK_NULL_HANDLE, "[Graphics::Swapchain] physical device handle is not set!");
	ASSERT(rPresentationSurface != VK_NULL_HANDLE, "[Graphics::Swapchain] surface handle is not set!");

	if (rLogicalDevice != VK_NULL_HANDLE) {
		vkDeviceWaitIdle(rLogicalDevice);
	}

	std::optional<VkPresentModeKHR> presentMode = selectPresentationMode();
	if (!presentMode.has_value())
	{
		return false;
	}

	VkSurfaceCapabilitiesKHR surfaceCapabilities;
	if (!getSurfaceCapabilities(surfaceCapabilities))
	{
		return false;
	}

	std::vector<VkSurfaceFormatKHR> surfaceFormats;
	if (!checkAvailableSurfaceFormats(surfaceFormats))
	{
		LOG("[Graphics::Swapchain] Could not enumerate Surface formates!");
		return false;
	}

	uint32_t numSwapImages = getSwapChainNumImages(surfaceCapabilities);
	VkExtent2D sizeOfImages = getSwapchainExtent(surfaceCapabilities, w, h);
	VkImageUsageFlags desiredUsages = getSwapChainUsageFlags(surfaceCapabilities);
	VkSurfaceTransformFlagBitsKHR surfaceTransform = surfaceCapabilities.supportedTransforms & VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR ? VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR : surfaceCapabilities.currentTransform;
	VkSurfaceFormatKHR surfaceFormat = getSwapchainFormat(surfaceFormats);
	VkSwapchainKHR oldSwapChain = mSwapChain;
	VkSwapchainCreateInfoKHR swapchain_create_info = {
	  VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
	  nullptr,
	  0,
	  rPresentationSurface,
	  numSwapImages,
	  surfaceFormat.format,
	  surfaceFormat.colorSpace,
	  sizeOfImages,
	  1,
	  desiredUsages,
	  VK_SHARING_MODE_EXCLUSIVE,
	  0,
	  nullptr,
	  surfaceTransform,
	  VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
	  *presentMode,
	  VK_TRUE,
	  oldSwapChain
	};

	VkResult result = vkCreateSwapchainKHR(rLogicalDevice, &swapchain_create_info, nullptr, &mSwapChain);
	if ((VK_SUCCESS != result) || (VK_NULL_HANDLE == mSwapChain)) 
	{
		LOG("[Graphics::Swapchain] Could not create a swapchain.");
		return false;
	}

	if (VK_NULL_HANDLE != oldSwapChain) 
	{
		vkDestroySwapchainKHR(rLogicalDevice, oldSwapChain, nullptr);
		oldSwapChain = VK_NULL_HANDLE;
	}

	if (!acquireSwapchainImages())
	{
		LOG("Could not acquire swap chain images!");
		return false;
	}


	return true;
}

std::optional<VkPresentModeKHR> SwapChain::selectPresentationMode()
{
	std::vector<VkPresentModeKHR> availablePresentModes;
	if (!checkAvailablePresentationModes(availablePresentModes))
	{
		ASSERT(false, "[Graphics::Swapchain] Could not enumerate available Presentation modes.");
		return std::nullopt;
	}

	for (auto&& presentationMode : availablePresentModes)
	{
		if (presentationMode == VK_PRESENT_MODE_MAILBOX_KHR)
		{
			return presentationMode;
		}
	}
	for (auto& current_present_mode : availablePresentModes) {
		if (current_present_mode == VK_PRESENT_MODE_FIFO_KHR) {
			return VK_PRESENT_MODE_FIFO_KHR;
		}
	}
	ASSERT(false, "[Graphics::Swapchain] Could not obtain default presentation mode, This should not be possible!");
	return std::nullopt;
}

bool SwapChain::getSurfaceCapabilities(VkSurfaceCapabilitiesKHR& surfaceCapabilities)
{
	if (vkGetPhysicalDeviceSurfaceCapabilitiesKHR(rPhysicalDevice, rPresentationSurface, &surfaceCapabilities) != VK_SUCCESS)
	{
		ASSERT(false, "[Graphics::Swapchain] Could not get the capabilities of the presentation surface!");
		return false;
	}
	return true;
}

uint32_t SwapChain::getSwapChainNumImages(const VkSurfaceCapabilitiesKHR& surfaceCapabilities)
{
	uint32_t numImages = surfaceCapabilities.minImageCount + 1;
	if (surfaceCapabilities.maxImageCount > 0 && numImages > surfaceCapabilities.maxImageCount)
	{
		numImages = surfaceCapabilities.maxImageCount;
	}
	return numImages;
}

VkExtent2D SwapChain::getSwapchainExtent(VkSurfaceCapabilitiesKHR surfaceCapabilities, uint32_t w, uint32_t h)
{
	VkExtent2D sizeOfImages;
	if (0xFFFFFFFF == surfaceCapabilities.currentExtent.width)
	{
		sizeOfImages = { w, h };
		SingularityEngine::Math::Clamp(sizeOfImages.width, surfaceCapabilities.minImageExtent.width, surfaceCapabilities.maxImageExtent.width);
		SingularityEngine::Math::Clamp(sizeOfImages.height, surfaceCapabilities.minImageExtent.height, surfaceCapabilities.maxImageExtent.height);
	}
	else
	{
		sizeOfImages = surfaceCapabilities.currentExtent;
	}
	return sizeOfImages;
}

VkImageUsageFlags SwapChain::getSwapChainUsageFlags(const VkSurfaceCapabilitiesKHR& surfaceCapabilities)
{
	VkImageUsageFlags desiredUsages = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
	VkImageUsageFlags imageUsage = desiredUsages & surfaceCapabilities.supportedUsageFlags;
	if (imageUsage != desiredUsages)
	{
		ASSERT(false, "[Graphics::Swapchain] Required usage Flags are not available!");
		return static_cast<VkImageUsageFlags>(-1);
	}
	return imageUsage;
}

VkSurfaceFormatKHR SwapChain::getSwapchainFormat(std::vector<VkSurfaceFormatKHR> surfaceFormats)
{
	VkSurfaceFormatKHR desiredSurfaceFormat;
	desiredSurfaceFormat.colorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
	desiredSurfaceFormat.format = VK_FORMAT_R8G8B8A8_UNORM;
	if (1 == surfaceFormats.size() && VK_FORMAT_UNDEFINED == surfaceFormats[0].format)
	{
		return desiredSurfaceFormat;
	}

	for (auto& surfaceFormat : surfaceFormats)
	{
		if ((desiredSurfaceFormat.format == surfaceFormat.format) && (desiredSurfaceFormat.colorSpace == surfaceFormat.colorSpace))
		{
			return surfaceFormat;
		}
	}

	for (auto& surfaceFormat : surfaceFormats)
	{
		if ((desiredSurfaceFormat.format == surfaceFormat.format))
		{
			LOG("[Graphics::Swapchain] Desired Surface color was not available, Selected other color space.");
			return surfaceFormat;
		}
	}
	LOG("[Graphics::Swapchain] Desired Surface color was not available, Selected other color space.");
	return surfaceFormats[0];
}

bool SwapChain::checkAvailableSurfaceFormats(std::vector<VkSurfaceFormatKHR>& surfaceFormats)
{
	uint32_t formatsCount;
	if (vkGetPhysicalDeviceSurfaceFormatsKHR(rPhysicalDevice, rPresentationSurface, &formatsCount, nullptr) != VK_SUCCESS || 0 == formatsCount)
	{
		ASSERT(false, "[Graphics::Swapchain] Could not get the number of supported surface formats.");
		return false;
	}
	surfaceFormats.resize(formatsCount);
	if (vkGetPhysicalDeviceSurfaceFormatsKHR(rPhysicalDevice, rPresentationSurface, &formatsCount, surfaceFormats.data()) != VK_SUCCESS || 0 == formatsCount)
	{
		ASSERT(false, "[Graphics::Swapchain] Could Enumerate available surface formats.");
		return false;
	}
	return true;
}

bool SwapChain::acquireSwapchainImage(VkSemaphore semaphore, VkFence fence, uint32_t& imageIndex)
{
	VkResult result;

	result = vkAcquireNextImageKHR(rLogicalDevice, mSwapChain, 2000000000, semaphore, fence, &imageIndex);
	switch (result) {
	case VK_SUCCESS:
	case VK_SUBOPTIMAL_KHR:
		return true;
	default:
		return false;
	}
}

bool SwapChain::acquireSwapchainImages()
{
	uint32_t imagesCount = 0;
	VkResult result = VK_SUCCESS;

	result = vkGetSwapchainImagesKHR(rLogicalDevice, mSwapChain, &imagesCount, nullptr);
	if ((VK_SUCCESS != result) || (0 == imagesCount)) 
	{
		LOG("[Graphics::Swapchain] Could not get the number of swapchain images.");
		return false;
	}

	mSwapChainImages.resize(imagesCount);
	result = vkGetSwapchainImagesKHR(rLogicalDevice, mSwapChain, &imagesCount, &mSwapChainImages[0]);
	if ((VK_SUCCESS != result) || (0 == imagesCount)) 
	{
		LOG("[Graphics::Swapchain] Could not enumerate swapchain images.");
		return false;
	}

	return true;
}

bool SwapChain::presentImage(VkQueue queue, std::vector<VkSemaphore> renderingSemaphores, std::vector<PresentInfo> imagesToPresent)
{
	std::vector<VkSwapchainKHR> swapchains;
	std::vector<uint32_t> imageIndicies;

	for (auto& imageToPresent : imagesToPresent)
	{
		swapchains.emplace_back(imageToPresent.swapchain);
		imageIndicies.emplace_back(imageToPresent.imageIndex);
	}

	VkPresentInfoKHR presentInfo = {
	  VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
	  nullptr,
	  static_cast<uint32_t>(renderingSemaphores.size()),
	  renderingSemaphores.size() > 0 ? &renderingSemaphores[0] : nullptr,
	  static_cast<uint32_t>(swapchains.size()),
	  swapchains.size() > 0 ? &swapchains[0] : nullptr,
	  swapchains.size() > 0 ? &imageIndicies[0] : nullptr,
	  nullptr
	};

	if (vkQueuePresentKHR(queue, &presentInfo) != VK_SUCCESS) return false;
	return true;
}

SwapChain::SwapChain(VkSurfaceKHR& presentSurface, VkDevice& logicalDevice, VkPhysicalDevice& physicalDevice) : rPhysicalDevice(physicalDevice), rLogicalDevice(logicalDevice), rPresentationSurface(presentSurface)
{

}

bool SwapChain::destroy()
{
	if (mSwapChain)
	{
		vkDestroySwapchainKHR(rLogicalDevice, mSwapChain, nullptr);
		mSwapChain = VK_NULL_HANDLE;
	}
	return true;
}