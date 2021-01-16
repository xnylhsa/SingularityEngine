#pragma once
#include "Common.h"

namespace SingularityEngine::Vulkan
{
	class CommandBuffers
	{
	public:
		CommandBuffers();
		bool Create(VkDevice& device, VkCommandPool& cmdPool, VkCommandBufferLevel level, uint32_t count);
		bool Destroy(VkDevice& device, VkCommandPool& cmdPool);
		bool beginRecording(uint32_t index, VkCommandBufferUsageFlags usage, VkCommandBufferInheritanceInfo* secondaryCommandBufferInfo);
		bool stopRecording(uint32_t index);
		bool resetBuffer(uint32_t index, bool releaseResources);
		std::vector<VkCommandBuffer>& get() { return mCommandBuffers; }
		VkCommandBuffer& operator[](size_t index) { return mCommandBuffers[index]; }
		VkCommandBuffer get(size_t index) { return mCommandBuffers[index]; }
	private:
		uint32_t mBufferCount = 0;
		std::vector<VkCommandBuffer> mCommandBuffers;
	};
}
