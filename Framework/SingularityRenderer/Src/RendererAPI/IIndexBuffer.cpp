#include "Precompiled.h"
#include "RendererAPI/IIndexBuffer.h"
#include "RendererAPI/Renderer.h"
#include "vulkan/Buffers/VulkanIndexBuffer.h"

namespace SingularityEngine::SERenderer
{
	std::shared_ptr<SingularityEngine::SERenderer::IIndexBuffer> IIndexBuffer::Create(size_t size)
	{
		switch (Renderer::GetAPI())
		{
		case RenderingAPI::None:    ASSERT(false, "[Renderer::Shader] None is currently not supported!"); return nullptr;
		case RenderingAPI::Vulkan:  return std::make_shared<VulkanIndexBuffer>(size);
		}
		//load into shader library
		ASSERT(false, "[Renderer::Shader] Unknown RendererAPI!");
		return nullptr;
	}

	std::shared_ptr<SingularityEngine::SERenderer::IIndexBuffer> IIndexBuffer::Create(void* data, size_t size)
	{
		switch (Renderer::GetAPI())
		{
		case RenderingAPI::None:    ASSERT(false, "[Renderer::Shader] None is currently not supported!"); return nullptr;
		case RenderingAPI::Vulkan:  return std::make_shared<VulkanIndexBuffer>(data, size);
		}
		//load into shader library
		ASSERT(false, "[Renderer::Shader] Unknown RendererAPI!");
		return nullptr;
	}

}