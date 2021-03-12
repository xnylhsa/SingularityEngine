#ifndef SINGULARITY_ENGINE_RENDERER_VULKAN_INDEX_BUFFER
#define SINGULARITY_ENGINE_RENDERER_VULKAN_INDEX_BUFFER
#include "RendererAPI/IBuffer.h"
#include "Vulkan/Memory/VulkanMemoryObject.h"

namespace SingularityEngine::SERenderer
{
	class VulkanBuffer : public IBuffer
	{
	public:
		VulkanBuffer(size_t size, size_t offset, BufferUsageType usageType);
		VulkanBuffer(void* data, size_t size, size_t offset, BufferUsageType usageType);
		~VulkanBuffer();
		void setData(void* buffer, size_t size, size_t offset = 0);
		void bind() const;

		size_t getCount() const override;
		size_t getSize() const override;
		size_t getOffset() const ;
	private:
		VkBufferUsageFlagBits convertUsageTypetoVKType(BufferUsageType usageType);
		size_t mSize;
		size_t mCount;
		size_t mOffset;
		void* mData;
		VkBuffer mBuffer;
		std::unique_ptr<VulkanMemoryObject> mMemoryObject;
		BufferUsageType mUsage;
	};
}
#endif