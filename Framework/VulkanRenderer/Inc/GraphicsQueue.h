#pragma once
#include "Common.h"


namespace SingularityEngine::Vulkan
{
	class GraphicsQueueFamily
	{
	public:
		GraphicsQueueFamily();
		bool hasQueueFamilyIndex();
		void setQueueFamilyIndex(uint32_t index);
		uint32_t getQueueFamilyIndex();
		void create(VkDevice& device);
		void destroy();
		VkQueue& get() { return mQueue; }
	private:
		VkQueue mQueue = VK_NULL_HANDLE;
		std::optional<uint32_t> mQueueFamilyIndex;
	};
}