#pragma once
#include "Common.h"

namespace SingularityEngine::Vulkan
{
	class Semaphore
	{
	public:
		Semaphore();
		bool create(VkDevice device);
		bool destroy(VkDevice device);
		bool get() { return mSemaphore; }
		~Semaphore();
	private:
		VkSemaphore mSemaphore;
	};
}