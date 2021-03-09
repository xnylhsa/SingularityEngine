#pragma once
#include "Common.h"

#include "RendererAPI/RendererContext.h"
#include "Vulkan/DescriptorInfos.h"
#include "Vulkan/Synchronization/VulkanFence.h"
#include "Vulkan/Texture/VulkanImage.h"
#include "Vulkan/Memory/VulkanMemoryAllocator.h"
#include "Vulkan/Buffers/VulkanCommandBuffers.h"
#include "Vulkan/Synchronization/VulkanCommandPool.h"
#include "Vulkan/Util/VulkanLibraryLoader.h"
#include "Vulkan/Util/VulkanParameters.h"
#include "Vulkan/Context/VulkanInstance.h"
#include "Vulkan/Device/VulkanDevice.h"
#include "Vulkan/Device/VulkanQueue.h"
#include "Vulkan/SwapChain/VulkanSwapChain.h"
#include "Vulkan/Synchronization/VulkanSemaphore.h"
#include "Vulkan/SwapChain/VulkanDepthStencil.h"
#include "Vulkan/RenderPass/VulkanRenderPass.h"
#include "Vulkan/Buffers/VulkanFrameBuffer.h"

namespace SingularityEngine::SERenderer
{
	//TODO create graphics base context to abstract and allow more renderers, such as dx11, dx12, opengl, metl etc.
	class VulkanContext : public SERenderer::IRenderContext
	{
	public:
		VulkanContext();
		~VulkanContext();
		static VulkanInstance* GetVulkanInstance();
		std::shared_ptr<VulkanDevice> GetVulkanDevice();
		bool isInitalized();
		DesiredExtensions& getDesiredDeviceExtensions() { return mDesiredDeviceExtensions; }

		bool initialize() override;
		bool teardown() override;
		bool setupDebugMessenger();


	protected:
		bool buildCommandBuffer();
		void fillDesiredInstanceExtensions(DesiredExtensions& extensions);
		void fillDesiredDeviceExtensions(DesiredExtensions& extensions);
	private:
		bool mIsInitalized = false;
		inline static std::unique_ptr<VulkanInstance>  sVulkanInstance = nullptr;
		std::shared_ptr<VulkanDevice> mVulkanDevice = nullptr;
		VkDebugUtilsMessengerEXT debugMessenger = VK_NULL_HANDLE;
		DesiredExtensions mDesiredDeviceExtensions;
	};
}