#include "precompiled.h"
#include "Semaphore.h"
#include "VulkanFunctions.h"
using namespace SingularityEngine::Vulkan;

Semaphore::Semaphore()
{

}

bool Semaphore::create(VkDevice device)
{
	VkSemaphoreCreateInfo semaphoreCreateInfo = {
	VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
	nullptr,
	0
	};
	if (vkCreateSemaphore(device, &semaphoreCreateInfo, nullptr, &mSemaphore) != VK_SUCCESS)
	{
		LOG("[Graphics::Semaphore] Failed creation!");
		return false;
	}
	return true;
}

bool Semaphore::destroy(VkDevice device)
{
	if (mSemaphore != VK_NULL_HANDLE)
	{
		vkDestroySemaphore(device, mSemaphore, nullptr);
		mSemaphore = VK_NULL_HANDLE;
	}
	return true;
}

Semaphore::~Semaphore()
{

}
