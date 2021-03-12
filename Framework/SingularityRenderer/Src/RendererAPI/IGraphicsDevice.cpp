#include "Precompiled.h"
#include "RendererAPI/IGraphicsDevice.h"
#include "RendererAPI/Renderer.h"
#include "vulkan/Device/VulkanDevice.h"

namespace SingularityEngine::SERenderer
{

	std::shared_ptr<SingularityEngine::SERenderer::IGraphicsDevice> IGraphicsDevice::Create()
	{
		switch (Renderer::GetAPI())
		{
		case RenderingAPI::None:    ASSERT(false, "[SERenderer::RendererAPI] None is currently not supported!"); return nullptr;
		case RenderingAPI::Vulkan:  return std::make_shared<VulkanDevice>();
		case RenderingAPI::DirectX12:    ASSERT(false, "[SERenderer::RendererAPI] DirectX12 is currently not supported!"); return nullptr;
		}

		ASSERT(false, "[SERenderer::RendererAPI] Unknown RendererAPI!");
		return nullptr;
	}

}


