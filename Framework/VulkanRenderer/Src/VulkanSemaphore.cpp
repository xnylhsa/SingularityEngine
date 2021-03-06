#include "precompiled.h"
#include "VulkanSemaphore.h"
#include "VulkanFunctions.h"
#include "VulkanContext.h"
using namespace SingularityEngine::Graphics;

VulkanSemaphore::VulkanSemaphore(std::string tag) : mTag(tag)
{
	std::shared_ptr<VulkanDevice> device = VulkanContext::GetVulkanDevice();
	VkSemaphoreCreateInfo semaphoreCreateInfo = {
VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
nullptr,
0
	};
	if (vkCreateSemaphore(*device, &semaphoreCreateInfo, nullptr, &mSemaphore) != VK_SUCCESS)
	{
		ASSERT(false, "[Graphics::Semaphore] Failed creation!");
	}
}

VulkanSemaphore::~VulkanSemaphore()
{
	if (mSemaphore != VK_NULL_HANDLE)
	{
		std::shared_ptr<VulkanDevice> device = VulkanContext::GetVulkanDevice();

		vkDestroySemaphore(*device, mSemaphore, nullptr);
		mSemaphore = VK_NULL_HANDLE;
	}
}
