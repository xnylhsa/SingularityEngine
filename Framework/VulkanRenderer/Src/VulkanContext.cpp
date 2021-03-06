#include "Precompiled.h"
#include "VulkanContext.h"
#include "vulkanFunctions.h"

#include "MemBuffer.h"
#include "VulkanMemoryObject.h"
using namespace SingularityEngine;
using namespace Graphics;

namespace
{
	VulkanContext* sContext = nullptr;
}


bool VulkanContext::isInitalized() 
{
	return sContext != nullptr;
}


void VulkanContext::StaticInitialize(StartupParameters& startupInfo)
{
	ASSERT(sContext == nullptr, "[Graphics::GraphicsSystem] System already initialized!");
	sContext = new VulkanContext();
	sContext->init(startupInfo);
}

void VulkanContext::StaticTerminate()
{
	if (sContext != nullptr)
	{
		sContext->shutdown();
		SafeDelete(sContext);
	}
}

bool VulkanContext::onWindowSizeChanged(uint32_t w, uint32_t h)
{
	ASSERT(mVulkanDevice->isValid(), "[GraphicsSystem] windowSize changed before device was created, creation of cmd buffers and swap chain will fail!");
	mSwapChain->onResize(w, h);
	//if ((width > 0.0f) && (height > 0.0f)) {
//	if (settings.overlay) {
//		UIOverlay.resize(width, height);
//	}
//}

	//buildCommandBuffers();
	mVulkanDevice->wait();
	return true;
}

VulkanContext* VulkanContext::Get()
{
	ASSERT(sContext != nullptr, "[Graphics::GraphicsSystem] No system registered.");
	return sContext;
}

VulkanContext::VulkanContext() : mVulkanLibrary()
{
}

VulkanContext::~VulkanContext()
{

}

bool VulkanContext::init(StartupParameters& startupInfo)
{
	//Clean this part up
	startupInfo.mDeviceExtensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
	startupInfo.mInstanceExtensions.push_back(VK_KHR_SURFACE_EXTENSION_NAME);
#if defined(_WIN32) || (_WIN64)
	startupInfo.mInstanceExtensions.push_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
#elif defined(VK_USE_PLATFORM_XCB_KHR)
	startupInfo.mInstanceExtensions.push_back(VK_KHR_XCB_SURFACE_EXTENSION_NAME);
#elif defined(VK_USE_PLATFORM_XLIB_KHR)
	startupInfo.mInstanceExtensions.push_back(VK_KHR_XLIB_SURFACE_EXTENSION_NAME);
#endif
	startupInfo.mDesiredDeviceFeatures.geometryShader = VK_TRUE;
	//^

	LOG("[Vulkan Renderer] Initalizing...");
	if (!mVulkanLibrary.init())
	{
		return false;
	}

	LOG("[Vulkan Renderer] Creating Instance...");
	if (enableValidationLayers) {
		startupInfo.mInstanceExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
	}
	mVulkanInstance = std::make_unique<VulkanInstance>(startupInfo);
	if (!mVulkanInstance->isValid())
	{
		return false;
	}
	LOG("[Vulkan Renderer] Instance Created...");

	if (!mVulkanLibrary.loadInstanceFuncs(*mVulkanInstance, startupInfo))
	{
		return false;
	}

	LOG("[Vulkan Renderer] Creating Device...");
	mVulkanDevice = std::make_shared<VulkanDevice>(startupInfo);
	if (!mVulkanDevice->isValid())
	{
		return false;
	}
	LOG("[Vulkan Renderer] Device Created...");
	if (!mVulkanLibrary.loadDeviceFuncs(*mVulkanDevice, startupInfo))
	{
		return false;
	}
	if (!setupDebugMessenger())
	{
		return false;
	}
	mVulkanDevice->createDeviceQueues();

	mAllocator = std::make_unique<VulkanMemoryAllocator>("Default");
	mSwapChain = std::make_unique<VulkanSwapChain>();
	ASSERT(mSwapChain, "[GraphicsSystem] Could not create Swap Chain pointer");
	LOG("[Vulkan Renderer] Creating SwapChain...");
	if (!mSwapChain->initalize(startupInfo))
	{
		return false;
	}
	LOG("[Vulkan Renderer] Presentation SwapChain...");
	LOG("[Vulkan Renderer] Initalized!");
	return true;
}

bool VulkanContext::shutdown()
{
	if (!mVulkanDevice->wait())
	{
		return false;
	}
	mSwapChain.reset();
	mAllocator.reset();
	mVulkanDevice->destroyDeviceQueues();
	mVulkanDevice.reset();
	if (enableValidationLayers) {
		vkDestroyDebugUtilsMessengerEXT(*mVulkanInstance, debugMessenger, nullptr);
	}
	mVulkanInstance.reset();
	mVulkanLibrary.shutdown();
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

	if (vkCreateDebugUtilsMessengerEXT(*mVulkanInstance, &createInfo, nullptr, &debugMessenger) != VK_SUCCESS)
	{
		LOG("Failed to load validation layers debug messenger!");
		return false;
	}

	return true;
}

bool VulkanContext::submitCommandBuffers(std::vector<WaitSemaphoreInfo> waitSemaphoreInfos, std::vector<VkSemaphore> signalSemaphores, std::vector<VkCommandBuffer>& commandBuffers, VkQueue& queue, VulkanFence fence)
{
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

	if (vkQueueSubmit(queue, 1, &submitInfo, fence) != VK_SUCCESS) {
		LOG("Error occurred during command buffer submission.");
		return false;
	}
	return true;
}

bool VulkanContext::submitSynchronizedCommandBuffers(std::vector<WaitSemaphoreInfo> synchronizedSemaphores, VkQueue& firstQueue, VkQueue& secondQueue, std::vector<WaitSemaphoreInfo> firstWaitSemaphoreInfos, std::vector<VkCommandBuffer>& firstCommandBuffers, std::vector<VkCommandBuffer>& secondCommandBuffers, VulkanFence& fence)
{
	std::vector<VkSemaphore> firstSignalSemaphores;
	for (auto& semaphoreInfo : synchronizedSemaphores)
	{
		firstSignalSemaphores.emplace_back(semaphoreInfo.semaphore);
	}

	VulkanFence syncFence;
	syncFence.create(false);
	if (!submitCommandBuffers(firstWaitSemaphoreInfos, firstSignalSemaphores, firstCommandBuffers, firstQueue, syncFence))
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

bool VulkanContext::checkIfSubmittedCommandBufferHasFinished(VkQueue& queue, std::vector<WaitSemaphoreInfo> waitInfos, std::vector<VkCommandBuffer>& commandBuffers, std::vector<VkSemaphore> signalSemaphores, VulkanFence& fence, uint64_t timeout)
{
	if (!submitCommandBuffers(waitInfos, signalSemaphores, commandBuffers, queue, fence))
	{
		return false;
	}


	return VulkanFence::waitForFences({ fence }, false, timeout);
}

bool VulkanContext::setBufferMemorybarrier(VkCommandBuffer cmdBuffer, VkPipelineStageFlags generatingStages, VkPipelineStageFlags consumingStages, std::vector<BufferTransition> bufferTransitions)
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

bool VulkanContext::setImageMemoryBarrier(VkCommandBuffer cmdBuffer, VkPipelineStageFlags generatingStages, VkPipelineStageFlags consumingStages, std::vector<ImageTransition> imageTransitions)
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

void VulkanContext::copyBufferData(VkCommandBuffer commandBuffer, VkBuffer sourceBuffer, VkBuffer destinationBuffer, std::vector<VkBufferCopy> regions)
{
	if (regions.size() > 0)
	{
		vkCmdCopyBuffer(commandBuffer, sourceBuffer, destinationBuffer, static_cast<uint32_t>(regions.size()), regions.data());
	}
}

bool VulkanContext::updateBufferWithDeviceLocalMemory(VkDeviceSize dataSize, void* data, VkBuffer destinationbuffer, VkDeviceSize destinationOffset,
	VkAccessFlags destinationBufferCurrentAccess, VkAccessFlags destinationBufferNewAccess, VkPipelineStageFlags destinationBufferGeneratingStages, 
	VkPipelineStageFlags destinationBufferConsumingStages, VkQueue queue, VulkanCommandBuffers commandBuffers, uint32_t cmdBufferIndex, std::vector<VkSemaphore> signalSemaphores)
{
	MemBuffer stagingBuffer;
	if (!stagingBuffer.create(dataSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT))
	{
		return false;
	}

	std::unique_ptr<VulkanMemoryObject> memoryObject = mAllocator->allocateBufferMemory(VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, stagingBuffer.get());
	if (!memoryObject)
	{
		return false;
	}

	if (!memoryObject->MapUpdateAndUnmapHostVisibleMemory(0, dataSize, data, true, nullptr))
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
	VulkanFence fence;
	fence.create(false);
	std::vector<VkCommandBuffer> cmdBuffersToSubmit = { commandBuffers.get(cmdBufferIndex) };
	if (!submitCommandBuffers({}, signalSemaphores, cmdBuffersToSubmit, queue, fence))
	{
		return false;
	}

	if (!VulkanFence::waitForFences({fence}, VK_FALSE, 500000000))
	{
		return false;
	}
	fence.destroy();
	stagingBuffer.destroy();
	memoryObject.reset();
	return true;
}

bool VulkanContext::updateImageWithDeviceLocalMemory(VkDeviceSize dataSize, void* data, VulkanImage destinationImage, VkImageSubresourceLayers destinationImageSubresource, VkOffset3D destinationImageOffset, VkExtent3D destinationImageSize, VkImageLayout destinationImageCurrentLayout, VkImageLayout destinationImageNewLayout, VkAccessFlags destinationImageCurrentAccess, VkAccessFlags destinationImageNewAccess, VkImageAspectFlags aspect, VkPipelineStageFlags dstImageGeneratingStages, VkPipelineStageFlags dstImageConsumingStages, VkQueue queue, VulkanCommandBuffers commandBuffers, uint32_t cmdBufferIndex, std::vector<VkSemaphore> signalSemaphores)
{
	MemBuffer stagingBuffer;
	if (!stagingBuffer.create(dataSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT))
	{
		return false;
	}

	std::unique_ptr<VulkanMemoryObject> memoryObject = mAllocator->allocateBufferMemory(VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, stagingBuffer.get());
	if (!memoryObject)
	{
		return false;
	}

	if (!memoryObject->MapUpdateAndUnmapHostVisibleMemory(0, dataSize, data, true, nullptr))
	{
		return false;
	}

	if (!commandBuffers.beginRecording(cmdBufferIndex, VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT, nullptr))
	{
		return false;
	}

	setImageMemoryBarrier(commandBuffers[cmdBufferIndex], dstImageGeneratingStages, VK_PIPELINE_STAGE_TRANSFER_BIT, {
	  {
		destinationImage,
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
		destinationImage,
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
	VulkanFence fence;
	fence.create(false);
	std::vector<VkCommandBuffer> cmdBuffersToSubmit = { commandBuffers.get(cmdBufferIndex) };
	if (!submitCommandBuffers({}, signalSemaphores, cmdBuffersToSubmit, queue, fence))
	{
		return false;
	}

	if (!VulkanFence::waitForFences({ fence }, VK_FALSE, 500000000))
	{
		return false;
	}

	fence.destroy();
	stagingBuffer.destroy();
	memoryObject.reset();

	return true;
}

void VulkanContext::updateDescriptorSets(std::vector<Descriptors::ImageDescriptorInfo> const& imageDescriptorInfos, std::vector<Descriptors::BufferDescriptorInfo> const& bufferDescriptorInfos, std::vector<Descriptors::TexelBufferDescriptorInfo> const& texelBufferDescriptorInfos, std::vector<Descriptors::CopyDescriptorInfo> const& copyDescriptorInfos)
{
	std::vector<VkWriteDescriptorSet> write_descriptors;
	std::vector<VkCopyDescriptorSet> copy_descriptors;

	// image descriptors
	for (auto& image_descriptor : imageDescriptorInfos) {
		write_descriptors.push_back({
		  VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,                                 // VkStructureType                  sType
		  nullptr,                                                                // const void                     * pNext
		  image_descriptor.TargetDescriptorSet,                                   // VkDescriptorSet                  dstSet
		  image_descriptor.TargetDescriptorBinding,                               // uint32_t                         dstBinding
		  image_descriptor.TargetArrayElement,                                    // uint32_t                         dstArrayElement
		  static_cast<uint32_t>(image_descriptor.ImageInfos.size()),              // uint32_t                         descriptorCount
		  image_descriptor.TargetDescriptorType,                                  // VkDescriptorType                 descriptorType
		  image_descriptor.ImageInfos.data(),                                     // const VkDescriptorImageInfo    * pImageInfo
		  nullptr,                                                                // const VkDescriptorBufferInfo   * pBufferInfo
		  nullptr                                                                 // const VkBufferView             * pTexelBufferView
			});
	}

	// buffer descriptors
	for (auto& buffer_descriptor : bufferDescriptorInfos) {
		write_descriptors.push_back({
		  VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,                                 // VkStructureType                  sType
		  nullptr,                                                                // const void                     * pNext
		  buffer_descriptor.TargetDescriptorSet,                                  // VkDescriptorSet                  dstSet
		  buffer_descriptor.TargetDescriptorBinding,                              // uint32_t                         dstBinding
		  buffer_descriptor.TargetArrayElement,                                   // uint32_t                         dstArrayElement
		  static_cast<uint32_t>(buffer_descriptor.BufferInfos.size()),            // uint32_t                         descriptorCount
		  buffer_descriptor.TargetDescriptorType,                                 // VkDescriptorType                 descriptorType
		  nullptr,                                                                // const VkDescriptorImageInfo    * pImageInfo
		  buffer_descriptor.BufferInfos.data(),                                   // const VkDescriptorBufferInfo   * pBufferInfo
		  nullptr                                                                 // const VkBufferView             * pTexelBufferView
			});
	}

	// texel buffer descriptors
	for (auto& texel_buffer_descriptor : texelBufferDescriptorInfos) {
		write_descriptors.push_back({
		  VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,                                 // VkStructureType                  sType
		  nullptr,                                                                // const void                     * pNext
		  texel_buffer_descriptor.TargetDescriptorSet,                            // VkDescriptorSet                  dstSet
		  texel_buffer_descriptor.TargetDescriptorBinding,                        // uint32_t                         dstBinding
		  texel_buffer_descriptor.TargetArrayElement,                             // uint32_t                         dstArrayElement
		  static_cast<uint32_t>(texel_buffer_descriptor.TexelBufferViews.size()), // uint32_t                         descriptorCount
		  texel_buffer_descriptor.TargetDescriptorType,                           // VkDescriptorType                 descriptorType
		  nullptr,                                                                // const VkDescriptorImageInfo    * pImageInfo
		  nullptr,                                                                // const VkDescriptorBufferInfo   * pBufferInfo
		  texel_buffer_descriptor.TexelBufferViews.data()                         // const VkBufferView             * pTexelBufferView
			});
	}

	// copy descriptors
	for (auto& copy_descriptor : copyDescriptorInfos) {
		copy_descriptors.push_back({
		  VK_STRUCTURE_TYPE_COPY_DESCRIPTOR_SET,                                  // VkStructureType    sType
		  nullptr,                                                                // const void       * pNext
		  copy_descriptor.SourceDescriptorSet,                                    // VkDescriptorSet    srcSet
		  copy_descriptor.SourceDescriptorBinding,                                // uint32_t           srcBinding
		  copy_descriptor.SourceArrayElement,                                     // uint32_t           srcArrayElement
		  copy_descriptor.TargetDescriptorSet,                                    // VkDescriptorSet    dstSet
		  copy_descriptor.TargetDescriptorBinding,                                // uint32_t           dstBinding
		  copy_descriptor.TargetArrayElement,                                     // uint32_t           dstArrayElement
		  copy_descriptor.DescriptorCount                                         // uint32_t           descriptorCount
			});
	}
	
	vkUpdateDescriptorSets(*mVulkanDevice, static_cast<uint32_t>(write_descriptors.size()), write_descriptors.data(), static_cast<uint32_t>(copy_descriptors.size()), copy_descriptors.data());
}

void VulkanContext::bindDescriptorSet(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineType, VkPipelineLayout pipelineLayout, std::vector<VkDescriptorSet>const& descriptorSets, uint32_t indexOfFirstSet, std::vector<uint32_t> const& dynamicOffsets)
{
	vkCmdBindDescriptorSets(commandBuffer, pipelineType, pipelineLayout, indexOfFirstSet, static_cast<uint32_t>(descriptorSets.size()), descriptorSets.data(), static_cast<uint32_t>(dynamicOffsets.size()), dynamicOffsets.data());
}

VulkanInstance* VulkanContext::GetVulkanInstance()
{
	ASSERT(sContext != nullptr, "[Graphics::VulkanContext] No context registered.");
	ASSERT(sContext->mVulkanInstance->isValid(), "[Graphics::VulkanContext] Vulkan Instance is invalid!");
	return sContext->mVulkanInstance.get();
}

std::shared_ptr<VulkanDevice> VulkanContext::GetVulkanDevice()
{
	ASSERT(sContext != nullptr, "[Graphics::VulkanContext] No context registered.");
	ASSERT(sContext->mVulkanDevice->isValid(), "[Graphics::VulkanContext] Vulkan Instance is invalid!");
	return sContext->mVulkanDevice;
}

VulkanSwapChain* VulkanContext::getSwapChain()
{
	ASSERT(sContext != nullptr, "[Graphics::VulkanContext] No context registered.");
	ASSERT(sContext->mSwapChain != nullptr, "[Graphics::VulkanContext] Vulkan Swapchain is invalid!");
	return sContext->mSwapChain.get();
}

bool VulkanContext::buildCommandBuffer()
{
	return true;
}

void VulkanContext::beginRender()
{
	mSwapChain->beginFrame({ 1.0f, 0.0f, 1.0f, 1.0f });

}

void VulkanContext::endRender()
{
	mSwapChain->endFrame();
	mSwapChain->present();
}
