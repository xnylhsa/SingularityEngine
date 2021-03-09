#include "Precompiled.h"
#include "RendererAPI/ISwapChain.h"
#include "Vulkan/SwapChain/VulkanSwapChain.h"
#include "RendererAPI/Renderer.h"
namespace SingularityEngine::SERenderer
{

	std::unique_ptr<SingularityEngine::SERenderer::ISwapChain> ISwapChain::Create()
	{
		switch (Renderer::GetAPI())
		{
		case RenderingAPI::None:    ASSERT(false, "[SERenderer::RendererAPI] None is currently not supported!"); return nullptr;
		case RenderingAPI::Vulkan:  return std::make_unique<VulkanSwapChain>();
		case RenderingAPI::DirectX12:    ASSERT(false, "[SERenderer::RendererAPI] DirectX12 is currently not supported!"); return nullptr;
		}

		ASSERT(false, "[SERenderer::RendererAPI] Unknown RendererAPI!");
		return nullptr;
	}

}