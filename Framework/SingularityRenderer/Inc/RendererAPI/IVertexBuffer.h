#ifndef SINGULARITY_ENGINE_RENDERER_API_VERTEX_BUFFER
#define SINGULARITY_ENGINE_RENDERER_API_VERTEX_BUFFER
#include "Common.h"
namespace SingularityEngine::SERenderer
{
	enum class VertexBufferUsage
	{
		None = 1,
		Static,
		Dynamic,
		Max
	};

	class IVertexBuffer
	{
	public:
		virtual ~IVertexBuffer() = default;
		static std::shared_ptr<IVertexBuffer> Create(size_t size, VertexBufferUsage usage);
		static std::shared_ptr<IVertexBuffer> Create(void* data, size_t size, VertexBufferUsage usage);


		virtual void setData(void* buffer, size_t size, size_t offset = 0) = 0;
		virtual void bind() const = 0;

		virtual size_t getSize() const = 0;
	};
}

#endif