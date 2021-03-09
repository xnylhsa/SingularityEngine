#include "precompiled.h"
#include "Vulkan/Synchronization/VulkanSemaphore.h"
#include "Vulkan/Util/VulkanFunctions.h"
#include "Vulkan/Device/VulkanDevice.h"
using namespace SingularityEngine::SERenderer;

VulkanSemaphore::VulkanSemaphore(std::weak_ptr<VulkanDevice> device, std::string tag) : mTag(tag), mDevice(device)
{
	auto vulkanDevice = mDevice.lock();
	ASSERT(vulkanDevice, "[SERenderer::VulkanImage] device lost.");
	VkSemaphoreCreateInfo semaphoreCreateInfo = {
VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
nullptr,
0
	};
	if (vkCreateSemaphore(*vulkanDevice, &semaphoreCreateInfo, nullptr, &mSemaphore) != VK_SUCCESS)
	{
		ASSERT(false, "[Graphics::Semaphore] Failed creation!");
	}
}

VulkanSemaphore::~VulkanSemaphore()
{
	auto vulkanDevice = mDevice.lock();
	ASSERT(vulkanDevice, "[SERenderer::VulkanImage] device lost.");
	if (vulkanDevice && mSemaphore != VK_NULL_HANDLE)
	{
		vkDestroySemaphore(*vulkanDevice, mSemaphore, nullptr);
		mSemaphore = VK_NULL_HANDLE;
		mDevice.reset();
	}
}
