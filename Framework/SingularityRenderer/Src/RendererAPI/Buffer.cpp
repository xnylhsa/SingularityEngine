#include "Precompiled.h"
#include "RendererAPI/Buffer.h"
#include "RendererAPI/Renderer.h"


namespace SingularityEngine::SERenderer
{
	std::shared_ptr<VertexBuffer> VertexBuffer::Create(uint32_t size)
	{
		UNREFERENCED_PARAMETER(size);

		switch (Renderer::GetAPI())
		{
		case RenderingAPI::None:    ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		//case API::Vulkan:  return std::make_shared<VulkanVertexBuffer>(size);
		}

		ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	std::shared_ptr<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size)
	{
		UNREFERENCED_PARAMETER(vertices);
		UNREFERENCED_PARAMETER(size);

		switch (Renderer::GetAPI())
		{
		case RenderingAPI::None:    ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		//case API::Vulkan:  return std::make_shared<VulkanVertexBuffer>(vertices, size);
		}

		ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	std::shared_ptr<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t size)
	{
		UNREFERENCED_PARAMETER(size);
		UNREFERENCED_PARAMETER(indices);

		switch (Renderer::GetAPI())
		{
		case RenderingAPI::None:    ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		//case API::Vulkan:  return std::make_shared<VulkanIndexBuffer>(indices, size);
		}

		ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	std::shared_ptr<IndexBuffer> ShortIndexBuffer::Create(uint16_t* indices, uint32_t size)
	{
		UNREFERENCED_PARAMETER(indices);
		UNREFERENCED_PARAMETER(size);

		switch (Renderer::GetAPI())
		{
		case RenderingAPI::None:    ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		//case API::Vulkan:  return std::make_shared<VulkanShortIndexBuffer>(indices, size);
		}

		ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}