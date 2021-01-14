#pragma once
#include "Common.h"
#include "Device.h"

namespace SingularityEngine::Vulkan
{
	class GraphicsQueue
	{
	public:
		GraphicsQueue(uint32_t index);
		void init(Device& device, uint32_t familyIndex);
		void shutdown();
	private:
		VkQueue mQueue = VK_NULL_HANDLE;
		uint32_t mIndex = 0;
	};

	class GraphicsQueueFamily
	{
	public:
		GraphicsQueueFamily(uint32_t familyIndex);
		void init(Device& device, std::vector<float> priorities);
		void shutdown();
	private:

		uint32_t mQueueFamilyIndex = 0;
		std::vector<GraphicsQueue> mQueues;
	};
}