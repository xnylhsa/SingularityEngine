#pragma once
#include "RendererAPI/ISwapChain.h"
#include "Vulkan/Util/VulkanParameters.h"
#include "Vulkan/Memory/VulkanMemoryAllocator.h"
#include "Vulkan/Buffers/VulkanCommandBuffers.h"
#include "Vulkan/Synchronization/VulkanCommandPool.h"
#include "Vulkan/Buffers/VulkanFrameBuffer.h"
#include "Vulkan/Synchronization/VulkanFence.h"
namespace SingularityEngine::SERenderer
{
	class VulkanDevice;
	class VulkanDepthStencil;
	class VulkanRenderPass;
	class VulkanSemaphore;
	class VulkanSwapChain : public ISwapChain
	{
	protected:
		struct SwapChainBuffer
		{
			VkImage image;
			VkImageView imageView;
		};
	public:
		VulkanSwapChain();
		~VulkanSwapChain();

		void onResize(uint32_t w, uint32_t h) override;
		bool initialize() override;
		void prepareFrame() override;
		void beginScreenFrame() override;
		void endScreenFrame() override;
		void present() override;
		void teardown() override;
		bool isValid() { return mSwapChain != VK_NULL_HANDLE; }
		VkExtent2D getSurfaceExtent() { ASSERT(mSurfaceExtent.has_value(), "[Graphics::Vulkan]failed to acquire surface extent!"); return mSurfaceExtent.value(); }

		VulkanRenderPass* getRenderpass() { return mRenderPass.get(); }

		VkCommandBuffer getCurrentCommandBuffer() { return mDrawCommandBuffers[mCurrentFrameIndex]; }
		VkFramebuffer getFrameBuffer() { return mFrameBuffers[mCurrentFrameIndex]; }

		operator VkSwapchainKHR() { return mSwapChain; }
	protected:

		bool queuePresent(VkQueue queue, uint32_t imageIndex, std::vector<VkSemaphore> renderSemaphores);
		VkResult acquireNextSwapchainImage();
		//creation
		void create(uint32_t w, uint32_t h);
		void createSwapChain(uint32_t w, uint32_t h);
		bool createDepthStencil();
		bool createFrameBuffer();
		bool createRenderPass();
		bool createSemaphores();
		bool createFences();
		bool createCommandPools();
		bool createCommandBuffers();
		//cleanup
		bool destroyCommandBuffers();

		//property acquisition from device
		bool acquireSwapchainImages();
		bool getSurfaceCapabilities(VkSurfaceCapabilitiesKHR& surfaceCapabilities);
		bool checkAvailablePresentationModes(std::vector<VkPresentModeKHR>& presentationModes);
		bool checkAvailableSurfaceFormats(std::vector<VkSurfaceFormatKHR>& surfaceFormats);
		uint32_t getSwapChainNumImages(const VkSurfaceCapabilitiesKHR& surfaceCapabilities);
		VkImageUsageFlags getSwapChainUsageFlags(const VkSurfaceCapabilitiesKHR& surfaceCapabilities);
		VkExtent2D getSwapchainExtent(VkSurfaceCapabilitiesKHR surfaceCapabilities, uint32_t w, uint32_t h);
		VkSurfaceFormatKHR getSwapchainFormat(std::vector<VkSurfaceFormatKHR> surfaceFormats);
		std::optional<VkPresentModeKHR> selectPresentationMode();
	private:
		std::unique_ptr<VulkanMemoryAllocator> mAllocator;
		VkSwapchainKHR mSwapChain = VK_NULL_HANDLE;
		VkSurfaceKHR mPresentationSurface = VK_NULL_HANDLE;
		std::weak_ptr<VulkanDevice> mDevice;
		std::unique_ptr<VulkanDepthStencil> mDepthStencil = nullptr;
		std::unique_ptr<VulkanRenderPass> mRenderPass;
		std::optional<VkSubmitInfo> mSubmitInfo = std::nullopt;
		//Swap chain properties
		VkColorSpaceKHR mColorSpace = VkColorSpaceKHR::VK_COLOR_SPACE_MAX_ENUM_KHR;
		VkFormat mColorFormat = VkFormat::VK_FORMAT_MAX_ENUM;
		std::optional<VkExtent2D> mSurfaceExtent = std::nullopt;
		std::optional<uint32_t> mNumImages = std::nullopt;
		VkImageUsageFlags mUsageflags = VkImageUsageFlagBits::VK_IMAGE_USAGE_FLAG_BITS_MAX_ENUM;
		VkSurfaceTransformFlagBitsKHR mSurfaceTransformFlags = VkSurfaceTransformFlagBitsKHR::VK_SURFACE_TRANSFORM_FLAG_BITS_MAX_ENUM_KHR;
		std::optional<uint32_t> mPresentQueueFamilyIndex = std::nullopt;
		VkPipelineStageFlags mSubmitPipelineStages;
		uint32_t mCurrentFrameIndex = 0;
		//swapchain buffers
		std::vector<VkImage> mSwapChainImages;
		std::vector<SwapChainBuffer> mSwapchainBuffer;
		std::vector<VulkanFrameBuffer> mFrameBuffers;
		VulkanCommandBuffers mDrawCommandBuffers;
		VulkanCommandPool mPresentCommandPool;

		//Synchronization Objects
		std::unique_ptr<VulkanSemaphore> mIsPresentComplete;
		std::unique_ptr<VulkanSemaphore> mIsRenderComplete;
		std::vector<VulkanFence> mWaitFences;

	};
}