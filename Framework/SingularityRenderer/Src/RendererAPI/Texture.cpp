#include "Precompiled.h"
#include "RendererAPI/Texture.h"
#include "RendererAPI/Renderer.h"


namespace SingularityEngine::SERenderer
{

	std::shared_ptr<Texture2D> Texture2D::Create(uint32_t width, uint32_t height)
	{
		switch (Renderer::GetAPI())
		{
		case RenderingAPI::None:    ASSERT(false, "[Renderer::Texture2D] None is currently not supported!"); return nullptr;
		case RenderingAPI::Vulkan:  ASSERT(false, "[Renderer::Texture2D] Vulkan is currently not supported!"); return nullptr;
		}

		UNREFERENCED_PARAMETER(width);
		UNREFERENCED_PARAMETER(height);

		ASSERT(false, "[Renderer::Shader] Unknown RendererAPI!");
		return nullptr;
	}

	std::shared_ptr<Texture2D> Texture2D::Create(const std::string& path)
	{
		switch (Renderer::GetAPI())
		{
		case RenderingAPI::None:    ASSERT(false, "[Renderer::Texture2D] None is currently not supported!"); return nullptr;
		case RenderingAPI::Vulkan:  ASSERT(false, "[Renderer::Texture2D] Vulkan is currently not supported!"); return nullptr;
		}

		UNREFERENCED_PARAMETER(path);
		ASSERT(false, "[Renderer::Shader] Unknown RendererAPI!");
		return nullptr;
	}

}