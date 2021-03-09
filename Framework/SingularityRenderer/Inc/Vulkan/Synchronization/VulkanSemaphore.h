#pragma once
#include "Common.h"

namespace SingularityEngine::SERenderer
{
	class VulkanDevice;
	class VulkanSemaphore
	{
	public:
		VulkanSemaphore(std::weak_ptr<VulkanDevice> device, std::string tag);
		operator VkSemaphore() { return mSemaphore; }
		operator VkSemaphore*() { return &mSemaphore; }

		~VulkanSemaphore();
	private:
		std::string mTag;
		VkSemaphore mSemaphore;
		std::weak_ptr<VulkanDevice> mDevice;
	};
}