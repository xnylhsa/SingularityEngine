#pragma once
#include "Common.h"
#include "DescriptorInfos.h"
#include "VulkanFence.h"
#include "VulkanImage.h"
#include "VulkanMemoryAllocator.h"
#include "VulkanCommandBuffers.h"
#include "VulkanCommandPool.h"
#include "VulkanLibraryLoader.h"
#include "VulkanParameters.h"
#include "VulkanInstance.h"
#include "VulkanDevice.h"
#include "VulkanQueue.h"
#include "VulkanSwapChain.h"
#include "VulkanSemaphore.h"
#include "VulkanDepthStencil.h"
#include "VulkanRenderPass.h"
#include "VulkanFrameBuffer.h"

namespace SingularityEngine::Graphics
{
	//TODO create graphics base context to abstract and allow more renderers, such as dx11, dx12, opengl, metl etc.
	class VulkanContext
	{
	public:
		VulkanContext();
		~VulkanContext();
		static void StaticInitialize(StartupParameters& startupInfo);
		static void StaticTerminate();
		static VulkanContext* Get();
		static VulkanInstance* GetVulkanInstance();
		static std::shared_ptr<VulkanDevice> GetVulkanDevice();
		static VulkanSwapChain* getSwapChain();
		static bool isInitalized();
		void beginRender();

		void endRender();
		bool onWindowSizeChanged(uint32_t w, uint32_t h);
		void updateDescriptorSets(std::vector<Descriptors::ImageDescriptorInfo> const& imageDescriptorInfos, std::vector<Descriptors::BufferDescriptorInfo> const& bufferDescriptorInfos, std::vector<Descriptors::TexelBufferDescriptorInfo> const& texelBufferDescriptorInfos, std::vector<Descriptors::CopyDescriptorInfo> const& copyDescriptorInfos);
	protected:
		bool init(StartupParameters& startupInfo);
		bool shutdown();
		bool setupDebugMessenger();
		bool buildCommandBuffer();





		//Weird stuff i should try to move off of this specific class
		bool submitCommandBuffers(std::vector<WaitSemaphoreInfo> waitSemaphoreInfos, std::vector<VkSemaphore> signalSemaphores, std::vector<VkCommandBuffer>& commandBuffers, VkQueue& queue, VulkanFence fence);
		bool submitSynchronizedCommandBuffers(std::vector<WaitSemaphoreInfo> synchronizedSemaphores, VkQueue& firstQueue, VkQueue& secondQueue, std::vector<WaitSemaphoreInfo> firstWaitSemaphoreInfos, std::vector<VkCommandBuffer>& firstCommandBuffers, std::vector<VkCommandBuffer>& secondCommandBuffers, VulkanFence& fence);
		bool checkIfSubmittedCommandBufferHasFinished(VkQueue& queue, std::vector<WaitSemaphoreInfo> waitInfos, std::vector<VkCommandBuffer>& commandBuffers, std::vector<VkSemaphore> signalSemaphores, VulkanFence& fence, uint64_t timeout);
		bool setBufferMemorybarrier(VkCommandBuffer cmdBuffer, VkPipelineStageFlags generatingStages, VkPipelineStageFlags consumingStages, std::vector<BufferTransition> bufferTransitions);
		bool setImageMemoryBarrier(VkCommandBuffer cmdBuffer, VkPipelineStageFlags generatingStages, VkPipelineStageFlags consumingStages, std::vector<ImageTransition> imageTransitions);
		void copyBufferData(VkCommandBuffer commandBuffer, VkBuffer sourceBuffer, VkBuffer destinationBuffer, std::vector<VkBufferCopy> regions);
		bool updateBufferWithDeviceLocalMemory(VkDeviceSize dataSize, void* data, VkBuffer destinationbuffer, VkDeviceSize destinationOffset, VkAccessFlags destinationBufferCurrentAccess, VkAccessFlags destinationBufferNewAccess, VkPipelineStageFlags destinationBufferGeneratingStages, VkPipelineStageFlags destinationBufferConsumingStages, VkQueue queue, VulkanCommandBuffers commandBuffers, uint32_t cmdBufferIndex, std::vector<VkSemaphore> signalSemaphores);
		bool updateImageWithDeviceLocalMemory(VkDeviceSize dataSize, void* data, VulkanImage destinationImage, VkImageSubresourceLayers destinationImageSubresource, VkOffset3D destinationImageOffset, VkExtent3D destinationImageSize, VkImageLayout destinationImageCurrentLayout, VkImageLayout destinationImageNewLayout, VkAccessFlags destinationImageCurrentAccess, VkAccessFlags destinationImageNewAccess, VkImageAspectFlags aspect, VkPipelineStageFlags dstImageGeneratingStages,VkPipelineStageFlags dstImageConsumingStages, VkQueue queue, VulkanCommandBuffers commandBuffers, uint32_t cmdBufferIndex, std::vector<VkSemaphore> signalSemaphores);
		void bindDescriptorSet(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineType, VkPipelineLayout pipelineLayout, std::vector<VkDescriptorSet>const& descriptorSets, uint32_t indexOfFirstSet, std::vector<uint32_t> const& dynamicOffsets);
	private:
		std::unique_ptr<VulkanInstance> mVulkanInstance = nullptr;
		VulkanLibraryLoader mVulkanLibrary;
		std::shared_ptr<VulkanDevice> mVulkanDevice = nullptr;
		std::unique_ptr<VulkanSwapChain> mSwapChain = nullptr;
		VkDebugUtilsMessengerEXT debugMessenger = VK_NULL_HANDLE;
		std::unique_ptr<VulkanMemoryAllocator> mAllocator;
	};
}