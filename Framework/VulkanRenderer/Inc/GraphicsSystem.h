#pragma once
#include "Common.h"
#include "LibraryLoader.h"
#include "VulkanParameters.h"
#include "Instance.h"
#include "Device.h"
#include "GraphicsQueue.h"
#include "PresentationSurface.h"
#include "SwapChain.h"
#include "Semaphore.h"
#include "Fence.h"
#include "CommandPool.h"
#include "CommandBuffers.h"
#include "Image.h"
#include "DescriptorInfos.h"

namespace SingularityEngine::Vulkan
{
	class GraphicsSystem
	{
	public:
		GraphicsSystem();
		~GraphicsSystem();
		static void StaticInitialize(StartupParameters& startupInfo);
		static void StaticTerminate();
		static GraphicsSystem* Get();
		static bool isInitalized();
		bool onWindowSizeChanged(uint32_t w, uint32_t h);
		void updateDescriptorSets(std::vector<Descriptors::ImageDescriptorInfo> const& imageDescriptorInfos, std::vector<Descriptors::BufferDescriptorInfo> const& bufferDescriptorInfos, std::vector<Descriptors::TexelBufferDescriptorInfo> const& texelBufferDescriptorInfos, std::vector<Descriptors::CopyDescriptorInfo> const& copyDescriptorInfos);
	protected:
		bool waitForFences(std::vector<VkFence> fences, bool waitForAll, uint64_t timeout);
		bool submitCommandBuffers(std::vector<WaitSemaphoreInfo> waitSemaphoreInfos, std::vector<VkSemaphore> signalSemaphores, std::vector<VkCommandBuffer>& commandBuffers, VkQueue& queue, Fence fence);
		bool submitSynchronizedCommandBuffers(std::vector<WaitSemaphoreInfo> synchronizedSemaphores, VkQueue& firstQueue, VkQueue& secondQueue, std::vector<WaitSemaphoreInfo> firstWaitSemaphoreInfos, std::vector<VkCommandBuffer>& firstCommandBuffers, std::vector<VkCommandBuffer>& secondCommandBuffers, Fence& fence);
		bool resetFences(std::vector<VkFence> fences);
		bool init(StartupParameters& startupInfo);
		bool shutdown();
		bool setupDebugMessenger();
		bool checkIfSubmittedCommandBufferHasFinished(VkQueue& queue, std::vector<WaitSemaphoreInfo> waitInfos, std::vector<VkCommandBuffer>& commandBuffers, std::vector<VkSemaphore> signalSemaphores, Fence& fence, uint64_t timeout);
		bool waitForQueue(VkQueue& queue);
		bool waitForDevice();
		bool setBufferMemorybarrier(VkCommandBuffer cmdBuffer, VkPipelineStageFlags generatingStages, VkPipelineStageFlags consumingStages, std::vector<BufferTransition> bufferTransitions);
		bool setImageMemoryBarrier(VkCommandBuffer cmdBuffer, VkPipelineStageFlags generatingStages, VkPipelineStageFlags consumingStages, std::vector<ImageTransition> imageTransitions);
		void copyBufferData(VkCommandBuffer commandBuffer, VkBuffer sourceBuffer, VkBuffer destinationBuffer, std::vector<VkBufferCopy> regions);
		bool updateBufferWithDeviceLocalMemory(VkDeviceSize dataSize, void* data, VkBuffer destinationbuffer, VkDeviceSize destinationOffset, VkAccessFlags destinationBufferCurrentAccess, VkAccessFlags destinationBufferNewAccess, VkPipelineStageFlags destinationBufferGeneratingStages, VkPipelineStageFlags destinationBufferConsumingStages, VkQueue queue, CommandBuffers commandBuffers, uint32_t cmdBufferIndex, std::vector<VkSemaphore> signalSemaphores);
		bool updateImageWithDeviceLocalMemory(VkDeviceSize dataSize, void* data, Image destinationImage, VkImageSubresourceLayers destinationImageSubresource, VkOffset3D destinationImageOffset, VkExtent3D destinationImageSize, VkImageLayout destinationImageCurrentLayout, VkImageLayout destinationImageNewLayout, VkAccessFlags destinationImageCurrentAccess, VkAccessFlags destinationImageNewAccess, VkImageAspectFlags aspect, VkPipelineStageFlags dstImageGeneratingStages, VkPipelineStageFlags dstImageConsumingStages, VkQueue queue, CommandBuffers commandBuffers, uint32_t cmdBufferIndex, std::vector<VkSemaphore> signalSemaphores);
		void bindDescriptorSet(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineType, VkPipelineLayout pipelineLayout, std::vector<VkDescriptorSet>const& descriptorSets, uint32_t indexOfFirstSet, std::vector<uint32_t> const& dynamicOffsets);
	private:
		PresentationSurface mPresentationSurface;
		std::unique_ptr<Device> mVulkanDevice = nullptr;
		Instance mVulkanInstance;
		LibraryLoader mVulkanLibrary;
		std::unique_ptr<SwapChain> mSwapChain = nullptr;
		VkDebugUtilsMessengerEXT debugMessenger;
	};
}