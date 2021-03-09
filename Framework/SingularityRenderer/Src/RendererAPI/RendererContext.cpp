#include "Precompiled.h"
#include "RendererAPI/RendererContext.h"
#include "RendererAPI/Renderer.h"
#include "Vulkan/Context/VulkanContext.h"
namespace SingularityEngine::SERenderer
{

	std::unique_ptr<IRenderContext> IRenderContext::Create()
	{
		switch (Renderer::GetAPI())
		{
		case RenderingAPI::None: ASSERT(false, "[Renderer::RendererContext] None is not a supported API. "); return nullptr;
		case RenderingAPI::Vulkan: return std::make_unique<VulkanContext>();
		}


		ASSERT(false, "Unknown Platform");
		return nullptr;
	}

}
