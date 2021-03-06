#pragma once
#include "Common.h"
#include "VulkanCommandPool.h"
namespace SingularityEngine::Graphics
{
	class VulkanCommandBuffers
	{
	public:
		VulkanCommandBuffers();
		bool Create(VulkanCommandPool& cmdPool, VkCommandBufferLevel level, uint32_t count);
		bool Destroy(VulkanCommandPool& cmdPool);
		bool beginRecording(uint32_t index, VkCommandBufferUsageFlags usage, VkCommandBufferInheritanceInfo* secondaryCommandBufferInfo);
		bool stopRecording(uint32_t index);
		bool resetBuffer(uint32_t index, bool releaseResources);
		std::vector<VkCommandBuffer>& get() { return mCommandBuffers; }
		operator std::vector<VkCommandBuffer>() { return mCommandBuffers; }
		VkCommandBuffer& operator[](size_t index) { return mCommandBuffers[index]; }
		VkCommandBuffer get(size_t index) { return mCommandBuffers[index]; }
		size_t getSize() { return mCommandBuffers.size(); }
	private:
		uint32_t mBufferCount = 0;
		std::vector<VkCommandBuffer> mCommandBuffers;
	};
}
