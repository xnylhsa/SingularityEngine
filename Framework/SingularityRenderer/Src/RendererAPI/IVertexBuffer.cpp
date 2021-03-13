#include "Precompiled.h"
#include "RendererAPI/IVertexBuffer.h"
#include "RendererAPI/Renderer.h"
#include "vulkan/Buffers/VulkanVertexBuffer.h"

namespace SingularityEngine::SERenderer
{
	std::shared_ptr<SingularityEngine::SERenderer::IVertexBuffer> IVertexBuffer::Create(size_t size, VertexBufferUsage usageType)
	{
		switch (Renderer::GetAPI())
		{
		case RenderingAPI::None:    ASSERT(false, "[Renderer::Shader] None is currently not supported!"); return nullptr;
		case RenderingAPI::Vulkan:  return std::make_shared<VulkanVertexBuffer>(size, usageType);
		}
		//load into shader library
		ASSERT(false, "[Renderer::Shader] Unknown RendererAPI!");
		return nullptr;
	}

	std::shared_ptr<SingularityEngine::SERenderer::IVertexBuffer> IVertexBuffer::Create(void* data, size_t size, VertexBufferUsage usageType)
	{
		switch (Renderer::GetAPI())
		{
		case RenderingAPI::None:    ASSERT(false, "[Renderer::Shader] None is currently not supported!"); return nullptr;
		case RenderingAPI::Vulkan:  return std::make_shared<VulkanVertexBuffer>(data, size, usageType);
		}
		//load into shader library
		ASSERT(false, "[Renderer::Shader] Unknown RendererAPI!");
		return nullptr;
	}

}