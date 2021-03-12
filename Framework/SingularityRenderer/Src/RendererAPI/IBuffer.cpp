#include "Precompiled.h"
#include "RendererAPI/IBuffer.h"
#include "RendererAPI/Renderer.h"
#include "vulkan/Buffers/VulkanBuffer.h"

namespace SingularityEngine::SERenderer
{
	std::shared_ptr<SingularityEngine::SERenderer::IBuffer> IBuffer::Create(size_t size, size_t offset, BufferUsageType usageType)
	{
		switch (Renderer::GetAPI())
		{
		case RenderingAPI::None:    ASSERT(false, "[Renderer::Shader] None is currently not supported!"); return nullptr;
		case RenderingAPI::Vulkan:  return std::make_shared<VulkanBuffer>(size, offset, usageType);
		}
		//load into shader library
		ASSERT(false, "[Renderer::Shader] Unknown RendererAPI!");
		return nullptr;
	}

	std::shared_ptr<SingularityEngine::SERenderer::IBuffer> IBuffer::Create(void* data, size_t size, size_t offset, BufferUsageType usageType)
	{
		switch (Renderer::GetAPI())
		{
		case RenderingAPI::None:    ASSERT(false, "[Renderer::Shader] None is currently not supported!"); return nullptr;
		case RenderingAPI::Vulkan:  return std::make_shared<VulkanBuffer>(data, size, offset, usageType);
		}
		//load into shader library
		ASSERT(false, "[Renderer::Shader] Unknown RendererAPI!");
		return nullptr;
	}

}