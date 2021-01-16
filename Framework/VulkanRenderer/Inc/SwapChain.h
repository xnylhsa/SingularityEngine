#pragma once

namespace SingularityEngine::Vulkan
{
	class SwapChain
	{
	public:
		SwapChain(VkSurfaceKHR& presentSurface, VkDevice& logicalDevice, VkPhysicalDevice& physicalDevice);
		bool create(uint32_t w, uint32_t h);
		bool destroy();
	private:
		struct PresentInfo {
			VkSwapchainKHR  swapchain;
			uint32_t        imageIndex;
		};
		bool presentImage(VkQueue queue, std::vector<VkSemaphore> renderingSemaphores, std::vector<PresentInfo> imagesToPresent);

		bool acquireSwapchainImage(VkSemaphore semaphore, VkFence fence, uint32_t& imageIndex);
		bool acquireSwapchainImages();
		bool getSurfaceCapabilities(VkSurfaceCapabilitiesKHR& surfaceCapabilities);
		bool checkAvailablePresentationModes(std::vector<VkPresentModeKHR>& presentationModes);
		bool checkAvailableSurfaceFormats(std::vector<VkSurfaceFormatKHR>& surfaceFormats);
		uint32_t getSwapChainNumImages(const VkSurfaceCapabilitiesKHR& surfaceCapabilities);
		VkImageUsageFlags getSwapChainUsageFlags(const VkSurfaceCapabilitiesKHR& surfaceCapabilities);
		VkExtent2D getSwapchainExtent(VkSurfaceCapabilitiesKHR surfaceCapabilities, uint32_t w, uint32_t h);
		VkSurfaceFormatKHR getSwapchainFormat(std::vector<VkSurfaceFormatKHR> surfaceFormats);
		std::optional<VkPresentModeKHR> selectPresentationMode();
		VkDevice& rLogicalDevice;
		VkSurfaceKHR& rPresentationSurface;
		VkPhysicalDevice& rPhysicalDevice;
		VkSwapchainKHR mSwapChain = VK_NULL_HANDLE;
		std::vector<VkImage> mSwapChainImages;

	};
}