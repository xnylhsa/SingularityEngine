#ifndef SINGULARITY_ENGINE_RENDER_BUFFER
#define SINGULARITY_ENGINE_RENDER_BUFFER
#include "Common.h"

namespace SingularityEngine::SERenderer
{
	enum class BufferUsageType
	{
		VertexBuffer = 0,
		IndexBuffer,
		ImageBuffer,
		BUFFERUSAGETYPEMAX
	};

	class IBuffer
	{
	public:
		static std::shared_ptr<IBuffer> Create(size_t size, size_t offset, BufferUsageType usageType);
		static std::shared_ptr<IBuffer> Create(void* data, size_t size, size_t offset, BufferUsageType usageType);


		virtual void setData(void* buffer, size_t size, size_t offset = 0) = 0;
		virtual void bind() const = 0;

		virtual size_t getCount() const = 0;
		virtual size_t getSize() const = 0;

	private:
	};
}

#endif 
