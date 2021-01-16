#include "Precompiled.h"
#include "GraphicsSystem.h"
#include "vulkanFunctions.h"

#include "MemBuffer.h"
#include "MemoryObject.h"
using namespace SingularityEngine;
using namespace Vulkan;

namespace
{
	GraphicsSystem* sGraphicsSystem = nullptr;
}


bool GraphicsSystem::isInitalized() 
{
	return sGraphicsSystem != nullptr;
}


void GraphicsSystem::StaticInitialize(StartupParameters& startupInfo)
{
	ASSERT(sGraphicsSystem == nullptr, "[Graphics::GraphicsSystem] System already initialized!");
	sGraphicsSystem = new GraphicsSystem();
	sGraphicsSystem->init(startupInfo);
}

void GraphicsSystem::StaticTerminate()
{
	if (sGraphicsSystem != nullptr)
	{
		sGraphicsSystem->shutdown();
		SafeDelete(sGraphicsSystem);
	}
}

bool GraphicsSystem::onWindowSizeChanged(uint32_t w, uint32_t h)
{
	ASSERT(mVulkanDevice->getLogicalDevice() != VK_NULL_HANDLE, "[GraphicsSystem] windowSize changed before device was created, creation of cmd buffers and swap chain will fail!");

	if (mSwapChain->create(w, h))
	{
		return false;
	}
	//if (mVulkanDevice.createCommandBuffers())
	//{

	//}
	return true;
}

GraphicsSystem* GraphicsSystem::Get()
{
	ASSERT(sGraphicsSystem != nullptr, "[Graphics::GraphicsSystem] No system registered.");
	return sGraphicsSystem;
}

GraphicsSystem::GraphicsSystem()
{

}

GraphicsSystem::~GraphicsSystem()
{

}

bool GraphicsSystem::init(StartupParameters& startupInfo)
{
	LOG("[Vulkan Renderer] Initalizing...");
	if (!mVulkanLibrary.init())
	{
		return false;
	}
	LOG("[Vulkan Renderer] Creating Instance...");
	if (enableValidationLayers) {
		startupInfo.mInstanceParameters.mDesiredExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
	}
	if (!mVulkanInstance.create(startupInfo))
	{
		return false;
	}
	LOG("[Vulkan Renderer] Instance Created...");
	if (!mVulkanLibrary.loadInstanceFuncs(mVulkanInstance(), startupInfo))
	{
		return false;
	}
	LOG("[Vulkan Renderer] Creating Presentation Surface...");

	if (!mPresentationSurface.init(mVulkanInstance(), startupInfo))
	{
		return false;
	}
	LOG("[Vulkan Renderer] Presentaiton Surface Created...");
	LOG("[Vulkan Renderer] Creating Device...");

	mVulkanDevice = std::make_unique<Device>(mPresentationSurface);
	if (!mVulkanDevice->create(startupInfo, mVulkanInstance))
	{
		return false;
	}
	LOG("[Vulkan Renderer] Device Created...");

	if (!mVulkanLibrary.loadDeviceFuncs(mVulkanDevice->getLogicalDevice(), startupInfo))
	{
		return false;
	}
	if (!setupDebugMessenger())
	{
		return false;
	}

	LOG("[Vulkan Renderer] Aquiring graphics queues...");
	if (!mVulkanDevice->createDeviceQueues())
	{
		return false;
	}
	LOG("[Vulkan Renderer] Aquired graphics queues.");

	mSwapChain = std::make_unique<SwapChain>(mPresentationSurface(), mVulkanDevice->getLogicalDevice(), mVulkanDevice->getPhysicalDevice());
	ASSERT(mSwapChain, "[GraphicsSystem] Could not create Swap Chain pointer");
	LOG("[Vulkan Renderer] Creating Swapchain...");
	if (!mSwapChain->create(startupInfo.windowParams.width, startupInfo.windowParams.height))
	{
		return false;
	}
	LOG("[Vulkan Renderer] Created Swapchain...");


	LOG("[Vulkan Renderer] Initalized!");
	return true;
}

bool GraphicsSystem::shutdown()
{
	if (!waitForDevice())
	{
		return false;
	}
	mSwapChain->destroy();
	mSwapChain.reset();
	mVulkanDevice->destroy();
	mVulkanDevice.reset();
	mPresentationSurface.terminate(mVulkanInstance());
	if (enableValidationLayers) {
		vkDestroyDebugUtilsMessengerEXT(mVulkanInstance(), debugMessenger, nullptr);
	}
	mVulkanInstance.destroy();
	mVulkanLibrary.shutdown();
	return true;
}

bool GraphicsSystem::setupDebugMessenger()
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

	if (vkCreateDebugUtilsMessengerEXT(mVulkanInstance(), &createInfo, nullptr, &debugMessenger) != VK_SUCCESS)
	{
		LOG("Failed to load validation layers debug messenger!");
		return false;
	}

	return true;
}

bool GraphicsSystem::waitForFences(std::vector<VkFence> fences, bool waitForAll, uint64_t timeout)
{
	if (fences.size() <= 0 && vkWaitForFences(mVulkanDevice->getLogicalDevice(), static_cast<uint32_t>(fences.size()), &fences[0], static_cast<VkBool32>(waitForAll), timeout) != VK_SUCCESS)
	{
		LOG("[Graphics::fences] Waiting on fences failed!");
		return false;
	}
	return true;
}

bool GraphicsSystem::waitForQueue(VkQueue& queue) 
{
	if (vkQueueWaitIdle(queue) != VK_SUCCESS)
	{
		LOG("[GraphicsSystem] Failed to wait for queue!");
		return false;
	}
	return true;
}

bool GraphicsSystem::waitForDevice()
{
	if (vkDeviceWaitIdle(mVulkanDevice->getLogicalDevice()) != VK_SUCCESS)
	{
		LOG("[GraphicsSystem] Failed to wait on device!");
		return false;
	}
	return true;
}

bool GraphicsSystem::resetFences(std::vector<VkFence> fences)
{
	if (fences.size() <= 0 && vkResetFences(mVulkanDevice->getLogicalDevice(), static_cast<uint32_t>(fences.size()), &fences[0]) != VK_SUCCESS) 
	{
		LOG("[Graphics::Fences] Failed to reset fences!");
		return false;
	}
	return true;
}

bool GraphicsSystem::submitCommandBuffers(std::vector<WaitSemaphoreInfo> waitSemaphoreInfos, std::vector<VkSemaphore> signalSemaphores, std::vector<VkCommandBuffer>& commandBuffers, VkQueue& queue, Fence fence)
{
	VkFence vkFence = fence.isInitialized() ? fence.get() : nullptr;
	std::vector<VkSemaphore> waitSemaphoreHandles;
	std::vector<VkPipelineStageFlags> waitSemaphoreFlags;
	for (auto&& waitSemaphoreInfo : waitSemaphoreInfos)
	{
		waitSemaphoreHandles.push_back(waitSemaphoreInfo.semaphore);
		waitSemaphoreFlags.push_back(waitSemaphoreInfo.waitingStage);
	}

	VkSubmitInfo submitInfo = {
		VK_STRUCTURE_TYPE_SUBMIT_INFO,
		nullptr,
		static_cast<uint32_t>(waitSemaphoreInfos.size()),
		waitSemaphoreHandles.size() > 0 ? &waitSemaphoreHandles[0] : nullptr,
		waitSemaphoreFlags.size() > 0 ? &waitSemaphoreFlags[0] : nullptr,
		static_cast<uint32_t>(commandBuffers.size()),
		commandBuffers.size() > 0 ? &commandBuffers[0] : nullptr,
		static_cast<uint32_t>(signalSemaphores.size()),
		signalSemaphores.size() > 0 ? &signalSemaphores[0] : nullptr
	};

	if (vkQueueSubmit(queue, 1, &submitInfo, vkFence) != VK_SUCCESS) {
		LOG("Error occurred during command buffer submission.");
		return false;
	}
	return true;
}

bool GraphicsSystem::submitSynchronizedCommandBuffers(std::vector<WaitSemaphoreInfo> synchronizedSemaphores, VkQueue& firstQueue, VkQueue& secondQueue, std::vector<WaitSemaphoreInfo> firstWaitSemaphoreInfos, std::vector<VkCommandBuffer>& firstCommandBuffers, std::vector<VkCommandBuffer>& secondCommandBuffers, Fence& fence)
{
	std::vector<VkSemaphore> firstSignalSemaphores;
	for (auto& semaphoreInfo : synchronizedSemaphores)
	{
		firstSignalSemaphores.emplace_back(semaphoreInfo.semaphore);
	}

	if (!submitCommandBuffers(firstWaitSemaphoreInfos, firstSignalSemaphores, firstCommandBuffers, firstQueue, Fence()))
	{
		return false;
	}

	std::vector<VkSemaphore> secondSignalSemaphores;
	for (auto& semaphoreInfo : synchronizedSemaphores) {
		secondSignalSemaphores.emplace_back(semaphoreInfo.semaphore);
	}
	if (!submitCommandBuffers(synchronizedSemaphores, secondSignalSemaphores, secondCommandBuffers, secondQueue, fence))
	{
		return false;
	}
	return true;
}

bool GraphicsSystem::checkIfSubmittedCommandBufferHasFinished(VkQueue& queue, std::vector<WaitSemaphoreInfo> waitInfos, std::vector<VkCommandBuffer>& commandBuffers, std::vector<VkSemaphore> signalSemaphores, Fence& fence, uint64_t timeout)
{
	if (!submitCommandBuffers(waitInfos, signalSemaphores, commandBuffers, queue, fence))
	{
		return false;
	}
	std::vector<VkFence> fences = { fence.get() };


	return waitForFences(fences, false, timeout);
}

bool GraphicsSystem::setBufferMemorybarrier(VkCommandBuffer cmdBuffer, VkPipelineStageFlags generatingStages, VkPipelineStageFlags consumingStages, std::vector<BufferTransition> bufferTransitions)
{
	std::vector<VkBufferMemoryBarrier> bufferMemoryBarriers;

	for (auto& bufferTransition : bufferTransitions)
	{
		bufferMemoryBarriers.push_back({
			VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER,
			nullptr,
			bufferTransition.currentAccess,
			bufferTransition.newAccess,
			bufferTransition.currentQueueFamily,
			bufferTransition.newQueueFamily,
			bufferTransition.buffer,
			0,
			VK_WHOLE_SIZE
			});
	}

	if (bufferMemoryBarriers.size() > 0)
	{
		vkCmdPipelineBarrier(cmdBuffer, generatingStages, consumingStages, 0, 0, nullptr, static_cast<uint32_t>(bufferMemoryBarriers.size()), bufferMemoryBarriers.data(), 0, nullptr);
		return true;
	}

	return false;
}

bool GraphicsSystem::setImageMemoryBarrier(VkCommandBuffer cmdBuffer, VkPipelineStageFlags generatingStages, VkPipelineStageFlags consumingStages, std::vector<ImageTransition> imageTransitions)
{
	std::vector<VkImageMemoryBarrier> imageMemoryBarriers;

	for (auto& imageTransition : imageTransitions) {
		imageMemoryBarriers.push_back({
		  VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,   // VkStructureType            sType
		  nullptr,                                  // const void               * pNext
		  imageTransition.currentAccess,           // VkAccessFlags              srcAccessMask
		  imageTransition.newAccess,               // VkAccessFlags              dstAccessMask
		  imageTransition.currentLayout,           // VkImageLayout              oldLayout
		  imageTransition.newLayout,               // VkImageLayout              newLayout
		  imageTransition.currentQueueFamily,      // uint32_t                   srcQueueFamilyIndex
		  imageTransition.newQueueFamily,          // uint32_t                   dstQueueFamilyIndex
		  imageTransition.image,                   // VkImage                    image
		  {                                         // VkImageSubresourceRange    subresourceRange
			imageTransition.aspect,                  // VkImageAspectFlags         aspectMask
			0,                                        // uint32_t                   baseMipLevel
			VK_REMAINING_MIP_LEVELS,                  // uint32_t                   levelCount
			0,                                        // uint32_t                   baseArrayLayer
			VK_REMAINING_ARRAY_LAYERS                 // uint32_t                   layerCount
		  }
			});
	}

	if (imageMemoryBarriers.size() > 0)
	{
		vkCmdPipelineBarrier(cmdBuffer, generatingStages, consumingStages, 0, 0, nullptr, 0, nullptr, static_cast<uint32_t>(imageMemoryBarriers.size()), imageMemoryBarriers.data());
		return true;
	}
	return false;
}

void GraphicsSystem::copyBufferData(VkCommandBuffer commandBuffer, VkBuffer sourceBuffer, VkBuffer destinationBuffer, std::vector<VkBufferCopy> regions)
{
	if (regions.size() > 0)
	{
		vkCmdCopyBuffer(commandBuffer, sourceBuffer, destinationBuffer, static_cast<uint32_t>(regions.size()), regions.data());
	}
}

bool GraphicsSystem::updateBufferWithDeviceLocalMemory(VkDeviceSize dataSize, void* data, VkBuffer destinationbuffer, VkDeviceSize destinationOffset,
	VkAccessFlags destinationBufferCurrentAccess, VkAccessFlags destinationBufferNewAccess, VkPipelineStageFlags destinationBufferGeneratingStages, 
	VkPipelineStageFlags destinationBufferConsumingStages, VkQueue queue, CommandBuffers commandBuffers, uint32_t cmdBufferIndex, std::vector<VkSemaphore> signalSemaphores)
{
	MemBuffer stagingBuffer;
	if (!stagingBuffer.create(mVulkanDevice->getLogicalDevice(), dataSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT))
	{
		return false;
	}

	MemoryObject memoryObject;
	if (!memoryObject.createBufferMemoryObject(mVulkanDevice->getPhysicalDevice(), mVulkanDevice->getLogicalDevice(), VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, stagingBuffer.get()))
	{
		return false;
	}

	if (!memoryObject.MapUpdateAndUnmapHostVisibleMemory(mVulkanDevice->getLogicalDevice(), 0, dataSize, data, true, nullptr))
	{
		return false;
	}

	if (!commandBuffers.beginRecording(cmdBufferIndex, VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT, nullptr))
	{
		return false;
	}

	setBufferMemorybarrier(commandBuffers[cmdBufferIndex], destinationBufferGeneratingStages, VK_PIPELINE_STAGE_TRANSFER_BIT, { {destinationbuffer, destinationBufferCurrentAccess, VK_ACCESS_TRANSFER_WRITE_BIT, VK_QUEUE_FAMILY_IGNORED, VK_QUEUE_FAMILY_IGNORED} });

	copyBufferData(commandBuffers[cmdBufferIndex], stagingBuffer.get(), destinationbuffer, { {0, destinationOffset, dataSize} });

	setBufferMemorybarrier(commandBuffers[cmdBufferIndex], VK_PIPELINE_STAGE_TRANSFER_BIT, destinationBufferConsumingStages, { {destinationbuffer, VK_ACCESS_TRANSFER_WRITE_BIT, destinationBufferNewAccess, VK_QUEUE_FAMILY_IGNORED, VK_QUEUE_FAMILY_IGNORED} });

	if (!commandBuffers.stopRecording(cmdBufferIndex))
	{
		return false;
	}
	Fence fence;
	if (!fence.create(mVulkanDevice->getLogicalDevice(), false))
	{
		return false;
	}
	std::vector<VkCommandBuffer> cmdBuffersToSubmit = { commandBuffers.get(cmdBufferIndex) };
	if (!submitCommandBuffers({}, signalSemaphores, cmdBuffersToSubmit, queue, fence))
	{
		return false;
	}

	if (!waitForFences({fence.get()}, VK_FALSE, 500000000))
	{
		return false;
	}

	stagingBuffer.destroy(mVulkanDevice->getLogicalDevice());
	memoryObject.destroy(mVulkanDevice->getLogicalDevice());

	return true;
}

bool GraphicsSystem::updateImageWithDeviceLocalMemory(VkDeviceSize dataSize, void* data, Image destinationImage, VkImageSubresourceLayers destinationImageSubresource, VkOffset3D destinationImageOffset, VkExtent3D destinationImageSize, VkImageLayout destinationImageCurrentLayout, VkImageLayout destinationImageNewLayout, VkAccessFlags destinationImageCurrentAccess, VkAccessFlags destinationImageNewAccess, VkImageAspectFlags aspect, VkPipelineStageFlags dstImageGeneratingStages, VkPipelineStageFlags dstImageConsumingStages, VkQueue queue, CommandBuffers commandBuffers, uint32_t cmdBufferIndex, std::vector<VkSemaphore> signalSemaphores)
{
	MemBuffer stagingBuffer;
	if (!stagingBuffer.create(mVulkanDevice->getLogicalDevice(), dataSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT))
	{
		return false;
	}

	MemoryObject memoryObject;
	if (!memoryObject.createBufferMemoryObject(mVulkanDevice->getPhysicalDevice(), mVulkanDevice->getLogicalDevice(), VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, stagingBuffer.get()))
	{
		return false;
	}

	if (!memoryObject.MapUpdateAndUnmapHostVisibleMemory(mVulkanDevice->getLogicalDevice(), 0, dataSize, data, true, nullptr))
	{
		return false;
	}

	if (!commandBuffers.beginRecording(cmdBufferIndex, VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT, nullptr))
	{
		return false;
	}

	setImageMemoryBarrier(commandBuffers[cmdBufferIndex], dstImageGeneratingStages, VK_PIPELINE_STAGE_TRANSFER_BIT, {
	  {
		destinationImage.getImage(),
		destinationImageCurrentAccess,
		VK_ACCESS_TRANSFER_WRITE_BIT,
		destinationImageCurrentLayout,
		VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
		VK_QUEUE_FAMILY_IGNORED,
		VK_QUEUE_FAMILY_IGNORED,
		aspect
	  } });

	destinationImage.copyDataFromBuffer(commandBuffers[cmdBufferIndex], stagingBuffer.get(), VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, { {
			0,
			0,
			0,
			destinationImageSubresource,
			destinationImageOffset,
			destinationImageSize
		} });

	setImageMemoryBarrier(commandBuffers[cmdBufferIndex], VK_PIPELINE_STAGE_TRANSFER_BIT, dstImageConsumingStages, {
	  {
		destinationImage.getImage(),
		VK_ACCESS_TRANSFER_WRITE_BIT,
		destinationImageNewAccess,
		VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
		destinationImageNewLayout,
		VK_QUEUE_FAMILY_IGNORED,
		VK_QUEUE_FAMILY_IGNORED,
		aspect
	  } });


	if (!commandBuffers.stopRecording(cmdBufferIndex))
	{
		return false;
	}
	Fence fence;
	if (!fence.create(mVulkanDevice->getLogicalDevice(), false))
	{
		return false;
	}
	std::vector<VkCommandBuffer> cmdBuffersToSubmit = { commandBuffers.get(cmdBufferIndex) };
	if (!submitCommandBuffers({}, signalSemaphores, cmdBuffersToSubmit, queue, fence))
	{
		return false;
	}

	if (!waitForFences({ fence.get() }, VK_FALSE, 500000000))
	{
		return false;
	}

	stagingBuffer.destroy(mVulkanDevice->getLogicalDevice());
	memoryObject.destroy(mVulkanDevice->getLogicalDevice());

	return true;
}
