#include "Precompiled.h"
#include "RendererAPI/FrameBuffer.h"
#include "RendererAPI/Renderer.h"

namespace SingularityEngine::SERenderer
{

	std::shared_ptr <Framebuffer> Framebuffer::Create(const FramebufferSpecification& spec)
	{
		UNREFERENCED_PARAMETER(spec);

		switch (Renderer::GetAPI())
		{
		case RenderingAPI::None:    ASSERT(false, "[Renderer::Framebuffer] None is currently not supported!"); return nullptr;
		//case API::Vulkan:  return std::make_shared<VulkanFramebuffer>(spec);
		}

		ASSERT(false, "[Renderer::Framebuffer] Unknown RendererAPI!");
		return nullptr;
	}

}