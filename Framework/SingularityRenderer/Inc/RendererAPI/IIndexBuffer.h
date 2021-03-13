#ifndef SINGULARITY_ENGINE_RENDERER_API_INDEX_BUFFER
#define SINGULARITY_ENGINE_RENDERER_API_INDEX_BUFFER
#include "Common.h"
namespace SingularityEngine::SERenderer
{
	class IIndexBuffer
	{
	public:
		virtual ~IIndexBuffer() = default;
		static std::shared_ptr<IIndexBuffer> Create(size_t size);
		static std::shared_ptr<IIndexBuffer> Create(void* data, size_t size);

		
		virtual void setData(void* buffer, size_t size, size_t offset = 0) = 0;
		virtual void bind() const = 0;

		virtual size_t getSize() const = 0;
	};
}

#endif