#ifndef SINGULARITY_ENGINE_RENDERER_VULKAN_VERTEX_BUFFER
#define SINGULARITY_ENGINE_RENDERER_VULKAN_VERTEX_BUFFER
#include "RendererAPI/IVertexBuffer.h"
#include "Vulkan/Memory/vk_mem_alloc.h"

namespace SingularityEngine::SERenderer
{
	class VulkanVertexBuffer : public IVertexBuffer
	{
	public:
		VulkanVertexBuffer(size_t size, VertexBufferUsage usageType = VertexBufferUsage::Dynamic);
		VulkanVertexBuffer(void* data, size_t size, VertexBufferUsage usageType = VertexBufferUsage::Static);
		~VulkanVertexBuffer();
		void setData(void* buffer, size_t size, size_t offset = 0);
		void bind() const;
		size_t getSize() const override;
	private:
		size_t mSize;
		size_t mCount;
		Core::Buffer mData;
		VkBuffer mBuffer;
		VmaAllocation mDeviceMemory;
		VertexBufferUsage mUsage;
	};
}
#endif