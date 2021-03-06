#pragma once
#include "Common.h"

namespace SingularityEngine::Graphics
{
	class VulkanSemaphore
	{
	public:
		VulkanSemaphore(std::string tag);
		operator VkSemaphore() { return mSemaphore; }
		operator VkSemaphore*() { return &mSemaphore; }

		~VulkanSemaphore();
	private:
		std::string mTag;
		VkSemaphore mSemaphore;
	};
}