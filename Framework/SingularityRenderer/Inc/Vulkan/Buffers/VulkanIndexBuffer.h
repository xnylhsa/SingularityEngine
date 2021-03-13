#ifndef SINGULARITY_ENGINE_RENDERER_VULKAN_INDEX_BUFFER
#define SINGULARITY_ENGINE_RENDERER_VULKAN_INDEX_BUFFER
#include "RendererAPI/IIndexBuffer.h"
#include "Vulkan/Memory/VulkanMemoryObject.h"

namespace SingularityEngine::SERenderer
{
	class VulkanIndexBuffer : public IIndexBuffer
	{
	public:
		VulkanIndexBuffer(size_t size);
		VulkanIndexBuffer(void* data, size_t size);
		~VulkanIndexBuffer();
		void setData(void* buffer, size_t size, size_t offset = 0);
		void bind() const;

		size_t getSize() const override;
	private:
		size_t mSize;
		Core::Buffer mData;
		VkBuffer mBuffer;
		std::unique_ptr<VulkanMemoryObject> mMemoryObject;
	};
}
#endif
