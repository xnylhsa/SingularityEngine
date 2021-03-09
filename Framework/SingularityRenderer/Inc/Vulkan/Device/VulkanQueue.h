#pragma once
#include "Common.h"


namespace SingularityEngine::SERenderer
{
	class VulkanQueue
	{
	public:
		VulkanQueue();
		bool hasQueueFamilyIndex();
		void setQueueFamilyIndex(uint32_t index);
		uint32_t getQueueFamilyIndex();
		void create(VkDevice& device);
		void destroy();
		bool wait();
		operator VkQueue() { return mQueue; }
	private:
		VkQueue mQueue = VK_NULL_HANDLE;
		std::optional<uint32_t> mQueueFamilyIndex;
	};
}