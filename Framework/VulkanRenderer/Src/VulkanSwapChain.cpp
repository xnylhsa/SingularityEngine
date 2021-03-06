#include "Precompiled.h"
#include "VulkanSwapChain.h"
#include "VulkanFunctions.h"
#include "VulkanContext.h"
#include "VulkanImageView.h"
#include "PlatformSurfaceCreation.h"
using namespace SingularityEngine::Graphics;

VulkanSwapChain::VulkanSwapChain(): mSubmitPipelineStages(VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT)
{
	mAllocator = std::make_unique<VulkanMemoryAllocator>("SwapChain");
	mDevice = VulkanContext::GetVulkanDevice();
}

bool VulkanSwapChain::initalize(StartupParameters startupInfo)
{
	createSemaphores();
	VkResult result = Platform::CreateSurface(startupInfo, &mPresentationSurface);
	if (result != VkResult::VK_SUCCESS || mPresentationSurface == VK_NULL_HANDLE)
	{
		mPresentationSurface = VK_NULL_HANDLE;
		return false;
	}

	uint32_t queueFamiliesCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(*mDevice, &queueFamiliesCount, nullptr);
	if (queueFamiliesCount == 0)
	{
		ASSERT(false, "Physical device %p does not have any queue families.", mDevice.get());
		return 0;
	}
	std::vector<VkQueueFamilyProperties> queueFamilyProperties(queueFamiliesCount);
	vkGetPhysicalDeviceQueueFamilyProperties(*mDevice, &queueFamiliesCount, queueFamilyProperties.data());
	vkGetPhysicalDeviceQueueFamilyProperties(*mDevice, &queueFamiliesCount, nullptr);
	if (queueFamiliesCount == 0)
	{
		ASSERT(false, "Physical device %p does not have any queue families.", mDevice.get());
		return 0;
	}
	std::optional<uint32_t> graphicsFamilyIndex;
	std::optional<uint32_t> presentFamilyIndex;

	for (size_t i = 0; i < queueFamilyProperties.size(); i++)
	{
		VkBool32 presentSupport = VK_FALSE;
		vkGetPhysicalDeviceSurfaceSupportKHR(*mDevice, static_cast<uint32_t>(i), mPresentationSurface, &presentSupport);
		VkQueueFamilyProperties& properties = queueFamilyProperties[i];

		if (properties.queueFlags & VK_QUEUE_GRAPHICS_BIT && !graphicsFamilyIndex.has_value())
		{
			graphicsFamilyIndex = static_cast<uint32_t>(i);
		}
		if (presentSupport && !presentFamilyIndex.has_value())
		{
			presentFamilyIndex = static_cast<uint32_t>(i);
		}
		if (properties.queueFlags & VK_QUEUE_GRAPHICS_BIT && presentSupport)
		{
			graphicsFamilyIndex = static_cast<uint32_t>(i);
			presentFamilyIndex = static_cast<uint32_t>(i);
		}
	}
	mPresentQueueFamilyIndex = presentFamilyIndex;

	//Find Image format and color space for surface.
	std::vector<VkSurfaceFormatKHR> surfaceFormats;
	if (!checkAvailableSurfaceFormats(surfaceFormats))
	{
		LOG("[Graphics::Swapchain] Could not enumerate Surface formates!");
		return false;
	}
	VkSurfaceFormatKHR surfaceFormat = getSwapchainFormat(surfaceFormats);
	mColorSpace = surfaceFormat.colorSpace;
	mColorFormat = surfaceFormat.format;
	createCommandPools();

	LOG("[Vulkan Renderer] Creating Swapchain...");
	create(startupInfo.mWindow->getWidth(), startupInfo.mWindow->getHeight());
	if (!isValid())
	{
		return false;
	}
	LOG("[Vulkan Renderer] Created Swapchain...");

	createCommandBuffers();
	createDepthStencil();
	createRenderPass();
	createFences();
	createFrameBuffer();
	return true;
}

void VulkanSwapChain::createSwapChain(uint32_t w, uint32_t h)
{
	mDevice->wait();

	std::optional<VkPresentModeKHR> presentMode = selectPresentationMode();
	if (!presentMode.has_value())
	{
		return;
	}
	if (!mNumImages.has_value() || !mSurfaceExtent.has_value() || mUsageflags == VK_IMAGE_USAGE_FLAG_BITS_MAX_ENUM || mSurfaceTransformFlags == VK_SURFACE_TRANSFORM_FLAG_BITS_MAX_ENUM_KHR)
	{
		VkSurfaceCapabilitiesKHR surfaceCapabilities;
		if (!getSurfaceCapabilities(surfaceCapabilities))
		{
			return;
		}
		mNumImages = getSwapChainNumImages(surfaceCapabilities);
		mSurfaceExtent = getSwapchainExtent(surfaceCapabilities, w, h);
		mUsageflags = getSwapChainUsageFlags(surfaceCapabilities);
		mSurfaceTransformFlags = surfaceCapabilities.supportedTransforms & VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR ? VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR : surfaceCapabilities.currentTransform;
	}
	else if (mSurfaceExtent->height != h || mSurfaceExtent->width != w)
	{
		VkSurfaceCapabilitiesKHR surfaceCapabilities;
		if (!getSurfaceCapabilities(surfaceCapabilities))
		{
			return;
		}
		mSurfaceExtent = getSwapchainExtent(surfaceCapabilities, w, h);
	}



	VkSwapchainKHR oldSwapChain = mSwapChain;
	VkSwapchainCreateInfoKHR swapchain_create_info = {
	  VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
	  nullptr,
	  0,
	  mPresentationSurface,
	  mNumImages.value(),
	  mColorFormat,
	  mColorSpace,
	  mSurfaceExtent.value(),
	  1,
	  mUsageflags,
	  VK_SHARING_MODE_EXCLUSIVE,
	  0,
	  nullptr,
	  mSurfaceTransformFlags,
	  VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
	  *presentMode,
	  VK_TRUE,
	  oldSwapChain
	};

	VkResult result = vkCreateSwapchainKHR(*mDevice, &swapchain_create_info, nullptr, &mSwapChain);
	if ((VK_SUCCESS != result) || (VK_NULL_HANDLE == mSwapChain))
	{
		LOG("[Graphics::Swapchain] Could not create a swapchain.");
		return;
	}

	if (VK_NULL_HANDLE != oldSwapChain)
	{
		vkDestroySwapchainKHR(*mDevice, oldSwapChain, nullptr);
		oldSwapChain = VK_NULL_HANDLE;
	}

	if (!acquireSwapchainImages())
	{
		LOG("Could not acquire swap chain images!");
		return;
	}
}

void VulkanSwapChain::create(uint32_t w, uint32_t h)
{
	createSwapChain(w, h);
}

VulkanSwapChain::~VulkanSwapChain()
{
	for (VulkanFrameBuffer& frameBuffer : mFrameBuffers)
	{
		frameBuffer.destroy();
	}
	mFrameBuffers.clear();
	for (VulkanFence& fence : mWaitFences)
	{
		fence.destroy();
	}
	mWaitFences.clear();
	mRenderPass.reset();
	mDepthStencil.reset();
	destroyCommandBuffers();
	if (mSwapChain)
	{
		for (uint32_t i = 0; i < mNumImages; i++)
		{
			vkDestroyImageView(*mDevice, mSwapchainBuffer[i].imageView, nullptr);
		}
		vkDestroySwapchainKHR(*mDevice, mSwapChain, nullptr);
		mDevice.reset();
		mSwapChain = VK_NULL_HANDLE;
	}
	mPresentCommandPool.destroy();
	if (mPresentationSurface)
	{
		LOG("[Graphics System] Destroying presentation surface");
		VulkanInstance* instance = VulkanContext::GetVulkanInstance();
		vkDestroySurfaceKHR(*instance, mPresentationSurface, nullptr);
		mPresentationSurface = VK_NULL_HANDLE;
	}
	mIsPresentComplete.reset();
	mIsRenderComplete.reset();
}

bool VulkanSwapChain::checkAvailablePresentationModes(std::vector<VkPresentModeKHR>& presentationModes)
{
	uint32_t modeCount = 0;
	VkResult result = vkGetPhysicalDeviceSurfacePresentModesKHR(*mDevice, mPresentationSurface, &modeCount, nullptr);
	if ((result != VK_SUCCESS) || modeCount == 0u)
	{
		ASSERT(false, "[Graphics::Swapchain] Could not get the number of available Presentation modes!");
		return false;
	}
	presentationModes.resize(modeCount);
	result = vkGetPhysicalDeviceSurfacePresentModesKHR(*mDevice, mPresentationSurface, &modeCount, &presentationModes[0]);
	if ((result != VK_SUCCESS) || modeCount == 0u)
	{
		ASSERT(false, "[Graphics::Swapchain] Could Not Enumerate Presentation modes!");
		return false;
	}

	return true;
}

std::optional<VkPresentModeKHR> VulkanSwapChain::selectPresentationMode()
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

bool VulkanSwapChain::getSurfaceCapabilities(VkSurfaceCapabilitiesKHR& surfaceCapabilities)
{
	if (vkGetPhysicalDeviceSurfaceCapabilitiesKHR(*mDevice, mPresentationSurface, &surfaceCapabilities) != VK_SUCCESS)
	{
		ASSERT(false, "[Graphics::Swapchain] Could not get the capabilities of the presentation surface!");
		return false;
	}
	return true;
}

uint32_t VulkanSwapChain::getSwapChainNumImages(const VkSurfaceCapabilitiesKHR& surfaceCapabilities)
{
	uint32_t numImages = surfaceCapabilities.minImageCount + 1;
	if (surfaceCapabilities.maxImageCount > 0 && numImages > surfaceCapabilities.maxImageCount)
	{
		numImages = surfaceCapabilities.maxImageCount;
	}
	return numImages;
}

VkExtent2D VulkanSwapChain::getSwapchainExtent(VkSurfaceCapabilitiesKHR surfaceCapabilities, uint32_t w, uint32_t h)
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

VkImageUsageFlags VulkanSwapChain::getSwapChainUsageFlags(const VkSurfaceCapabilitiesKHR& surfaceCapabilities)
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

VkSurfaceFormatKHR VulkanSwapChain::getSwapchainFormat(std::vector<VkSurfaceFormatKHR> surfaceFormats)
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

bool VulkanSwapChain::checkAvailableSurfaceFormats(std::vector<VkSurfaceFormatKHR>& surfaceFormats)
{
	uint32_t formatsCount;
	if (vkGetPhysicalDeviceSurfaceFormatsKHR(*mDevice, mPresentationSurface, &formatsCount, nullptr) != VK_SUCCESS || 0 == formatsCount)
	{
		ASSERT(false, "[Graphics::Swapchain] Could not get the number of supported surface formats.");
		return false;
	}
	surfaceFormats.resize(formatsCount);
	if (vkGetPhysicalDeviceSurfaceFormatsKHR(*mDevice, mPresentationSurface, &formatsCount, surfaceFormats.data()) != VK_SUCCESS || 0 == formatsCount)
	{
		ASSERT(false, "[Graphics::Swapchain] Could Enumerate available surface formats.");
		return false;
	}
	return true;
}

VkResult VulkanSwapChain::acquireNextSwapchainImage()
{
	return vkAcquireNextImageKHR(*mDevice, mSwapChain, 2000000000, *mIsPresentComplete, VK_NULL_HANDLE, &mCurrentFrameIndex);
}

bool VulkanSwapChain::acquireSwapchainImages()
{
	mNumImages = 0;
	VkResult result = VK_SUCCESS;
	result = vkGetSwapchainImagesKHR(*mDevice, mSwapChain, &mNumImages.value(), nullptr);
	if ((VK_SUCCESS != result) || (0u == mNumImages))
	{
		LOG("[Graphics::Swapchain] Could not get the number of swapchain images.");
		return false;
	}

	if (!mSwapChainImages.empty())
	{
		for (uint32_t i = 0; i < mNumImages; i++)
		{
			vkDestroyImageView(*mDevice, mSwapchainBuffer[i].imageView, nullptr);
		}
		mSwapChainImages.clear();
	}
	mSwapChainImages.resize(mNumImages.value());
	result = vkGetSwapchainImagesKHR(*mDevice, mSwapChain, &mNumImages.value(), &mSwapChainImages[0]);
	if ((VK_SUCCESS != result) || (0u == mNumImages))
	{
		LOG("[Graphics::Swapchain] Could not enumerate swapchain images.");
		return false;
	}
	mSwapchainBuffer.resize(mNumImages.value());
	for (uint32_t i = 0; i < mNumImages; i++)
	{
		VkImageViewCreateInfo colorAttachmentView = {};
		colorAttachmentView.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		colorAttachmentView.pNext = nullptr;
		colorAttachmentView.format = mColorFormat;
		colorAttachmentView.components = {
			VkComponentSwizzle::VK_COMPONENT_SWIZZLE_R,
			VkComponentSwizzle::VK_COMPONENT_SWIZZLE_G,
			VkComponentSwizzle::VK_COMPONENT_SWIZZLE_B,
			VkComponentSwizzle::VK_COMPONENT_SWIZZLE_A
		};
		colorAttachmentView.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		colorAttachmentView.subresourceRange.baseMipLevel = 0;
		colorAttachmentView.subresourceRange.levelCount = 1;
		colorAttachmentView.subresourceRange.baseArrayLayer = 0;
		colorAttachmentView.subresourceRange.layerCount = 1;
		colorAttachmentView.viewType = VK_IMAGE_VIEW_TYPE_2D;
		colorAttachmentView.flags = 0;

		mSwapchainBuffer[i].image = mSwapChainImages[i];

		colorAttachmentView.image = mSwapchainBuffer[i].image;

		vkCreateImageView(*mDevice, &colorAttachmentView, nullptr, &mSwapchainBuffer[i].imageView);
	}

	return true;
}

void VulkanSwapChain::present()
{
	
	mSubmitInfo->pCommandBuffers = &mDrawCommandBuffers[mCurrentFrameIndex];
	mSubmitInfo->commandBufferCount = 1;
	if (vkQueueSubmit(mDevice->getGraphicsQueue(), 1, &mSubmitInfo.value(), mWaitFences[mCurrentFrameIndex]) != VK_SUCCESS) return;
	if (!queuePresent(mDevice->getGraphicsQueue(), mCurrentFrameIndex, { *mIsRenderComplete })) return;
	std::vector<VkFence> waitFences(mWaitFences.size());
	for (size_t i = 0; i < waitFences.size(); ++i)
	{
		waitFences[i] = mWaitFences[i];
	}
	VulkanFence::waitForFences(waitFences, true, 100000000000);
}



void VulkanSwapChain::onResize(uint32_t w, uint32_t h)
{
	create(w, h);
	mDepthStencil.reset();
	createDepthStencil();
	for (size_t i = 0; i <mFrameBuffers.size(); ++i)
	{
		mFrameBuffers[i].destroy();
	}
	mFrameBuffers.clear();
	createFrameBuffer();
	destroyCommandBuffers();
	createCommandBuffers();
}

bool VulkanSwapChain::createDepthStencil()
{
	mDepthStencil = std::make_unique<VulkanDepthStencil>(mAllocator.get());
	ASSERT(mDepthStencil, "[Graphics::Vulkan] Depth Stencil failed creation!");
	return mDepthStencil != nullptr;
}

bool VulkanSwapChain::createFrameBuffer()
{
	std::vector<VkImageView> ivAttachments(2);
	ivAttachments[1] = (*mDepthStencil->getImageView());
	uint32_t numImages = mNumImages.value();
	uint32_t width = mSurfaceExtent->width;
	uint32_t height = mSurfaceExtent->height;
	mFrameBuffers.resize(numImages);
	for (uint32_t i = 0; i < numImages; i++)
	{
		ivAttachments[0] = mSwapchainBuffer[i].imageView;
		mFrameBuffers[i].create(*mRenderPass, ivAttachments, width, height, 1);
	}
	return true;
}

bool VulkanSwapChain::createCommandPools()
{
	return mPresentCommandPool.create(mPresentQueueFamilyIndex.value(), VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT | VK_COMMAND_POOL_CREATE_TRANSIENT_BIT);
}

bool VulkanSwapChain::createCommandBuffers()
{
	return mDrawCommandBuffers.Create(mPresentCommandPool, VK_COMMAND_BUFFER_LEVEL_PRIMARY, mNumImages.value());
}

bool VulkanSwapChain::destroyCommandBuffers()
{
	return mDrawCommandBuffers.Destroy(mPresentCommandPool);
}

bool VulkanSwapChain::createRenderPass()
{
	std::vector<VkAttachmentDescription> attachments(2);
	// Color attachment
	attachments[0].format = mColorFormat;
	attachments[0].samples = VK_SAMPLE_COUNT_1_BIT;
	attachments[0].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	attachments[0].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	attachments[0].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	attachments[0].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	attachments[0].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	attachments[0].finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
	// Depth attachment
	attachments[1].format = mDepthStencil->getDepthStencilFormat();
	attachments[1].samples = VK_SAMPLE_COUNT_1_BIT;
	attachments[1].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	attachments[1].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	attachments[1].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	attachments[1].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	attachments[1].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	attachments[1].finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	VkAttachmentReference colorReference = {};
	colorReference.attachment = 0;
	colorReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkAttachmentReference depthReference = {};
	depthReference.attachment = 1;
	depthReference.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
	std::vector<VkSubpassDescription> subpassDescriptions(1);
	subpassDescriptions[0].pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpassDescriptions[0].colorAttachmentCount = 1;
	subpassDescriptions[0].pColorAttachments = &colorReference;
	subpassDescriptions[0].pDepthStencilAttachment = &depthReference;
	subpassDescriptions[0].inputAttachmentCount = 0;
	subpassDescriptions[0].pInputAttachments = nullptr;
	subpassDescriptions[0].preserveAttachmentCount = 0;
	subpassDescriptions[0].pPreserveAttachments = nullptr;
	subpassDescriptions[0].pResolveAttachments = nullptr;

	// Subpass dependencies for layout transitions
	std::vector<VkSubpassDependency> dependencies(2);

	dependencies[0].srcSubpass = VK_SUBPASS_EXTERNAL;
	dependencies[0].dstSubpass = 0;
	dependencies[0].srcStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
	dependencies[0].dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependencies[0].srcAccessMask = VK_ACCESS_MEMORY_READ_BIT;
	dependencies[0].dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
	dependencies[0].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

	dependencies[1].srcSubpass = 0;
	dependencies[1].dstSubpass = VK_SUBPASS_EXTERNAL;
	dependencies[1].srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependencies[1].dstStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
	dependencies[1].srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
	dependencies[1].dstAccessMask = VK_ACCESS_MEMORY_READ_BIT;
	dependencies[1].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;
	mRenderPass = std::make_unique<VulkanRenderPass>(attachments, subpassDescriptions, dependencies);
	return mRenderPass != nullptr;
}

bool VulkanSwapChain::createSemaphores()
{
	mIsRenderComplete = std::make_unique<VulkanSemaphore>("is render complete");
	mIsPresentComplete = std::make_unique<VulkanSemaphore>("is present complete");

	// Set up submit info structure
	// Semaphores will stay the same during application lifetime
	VkSubmitInfo submitInfo;
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.pNext = nullptr;
	submitInfo.pWaitDstStageMask = &mSubmitPipelineStages;
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = *mIsPresentComplete;
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = *mIsRenderComplete;
	mSubmitInfo = submitInfo;
	return mIsRenderComplete != nullptr && mIsPresentComplete != nullptr;
}

bool VulkanSwapChain::createFences()
{
	size_t drawCmdBufferSize = mDrawCommandBuffers.getSize();
	mWaitFences.resize(drawCmdBufferSize);
	for (size_t i = 0; i < drawCmdBufferSize; i++)
	{
		mWaitFences[i].create(true);
	}
	return true;
}



void VulkanSwapChain::beginFrame(Math::Color clearColor)
{
	VkResult result = acquireNextSwapchainImage();
	if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR)
	{
		onResize(mSurfaceExtent->width, mSurfaceExtent->height);
	}

	VulkanFence::waitForFences({ mWaitFences[mCurrentFrameIndex] }, true, UINT64_MAX);
	VulkanFence::resetFences({ mWaitFences[mCurrentFrameIndex] });

	mDrawCommandBuffers.beginRecording(mCurrentFrameIndex, 0, nullptr);
	std::vector<VkClearValue> clearValues(2);
	clearValues[0].color = { {clearColor.r, clearColor.g, clearColor.b, clearColor.a} };
	clearValues[1].depthStencil = { 1.0f, 0 };
	VkRect2D renderArea;
	renderArea.extent.width = mSurfaceExtent->width;
	renderArea.extent.height = mSurfaceExtent->height;
	renderArea.offset.x = 0;
	renderArea.offset.y = 0;
	mRenderPass->beginRendering(mDrawCommandBuffers[mCurrentFrameIndex], mFrameBuffers[mCurrentFrameIndex], renderArea, clearValues, VK_SUBPASS_CONTENTS_INLINE);
	VkViewport viewport = {};
	viewport.height = static_cast<float>(mSurfaceExtent->height);
	viewport.width = static_cast<float>(mSurfaceExtent->width);
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;
	vkCmdSetViewport(mDrawCommandBuffers[mCurrentFrameIndex], 0, 1, &viewport);
	VkRect2D scissor = {};
	scissor.extent.width = mSurfaceExtent->width;
	scissor.extent.height = mSurfaceExtent->height;
	scissor.offset.x = 0;
	scissor.offset.y = 0;
	vkCmdSetScissor(mDrawCommandBuffers[mCurrentFrameIndex], 0, 1, &scissor);
	//draw stuffs
}

void VulkanSwapChain::endFrame()
{
	mRenderPass->endRendering(mDrawCommandBuffers[mCurrentFrameIndex]);
	mDrawCommandBuffers.stopRecording(mCurrentFrameIndex);
}

bool VulkanSwapChain::queuePresent(VkQueue queue, uint32_t imageIndex, std::vector<VkSemaphore> renderSemaphores)
{
	VkPresentInfoKHR presentInfo = {};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	presentInfo.pNext = nullptr;
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = &mSwapChain;
	presentInfo.pImageIndices = &imageIndex;
	if (!renderSemaphores.empty())
	{
		presentInfo.pWaitSemaphores = renderSemaphores.data();
		presentInfo.waitSemaphoreCount = static_cast<uint32_t>(renderSemaphores.size());
	}
	VkResult success = vkQueuePresentKHR(queue, &presentInfo);
	if (success == VK_SUBOPTIMAL_KHR || success != VK_SUCCESS)
	{
		if (success == VK_ERROR_OUT_OF_DATE_KHR)
		{
			onResize(mSurfaceExtent->width, mSurfaceExtent->height);
			return false;
		}
	}
	return true;
}
