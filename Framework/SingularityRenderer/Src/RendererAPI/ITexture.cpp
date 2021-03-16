#include "Precompiled.h"
#include "RendererAPI/ITexture.h"
#include "RendererAPI/Renderer.h"


namespace SingularityEngine::SERenderer
{

	std::shared_ptr<ITexture2D> ITexture2D::Create(TextureFormat format, uint32_t width, uint32_t height, const void* data, TextureWrapMode wrapMode)
	{
		switch (Renderer::GetAPI())
		{
		case RenderingAPI::None:    ASSERT(false, "[Renderer::Texture2D] None is currently not supported!"); return nullptr;
		case RenderingAPI::Vulkan:  ASSERT(false, "[Renderer::Texture2D] Vulkan is currently not supported!"); return nullptr;
		}

		UNREFERENCED_PARAMETER(width);
		UNREFERENCED_PARAMETER(height);
		UNREFERENCED_PARAMETER(format);
		UNREFERENCED_PARAMETER(wrapMode);
		UNREFERENCED_PARAMETER(data);

		ASSERT(false, "[Renderer::Shader] Unknown RendererAPI!");
		return nullptr;
	}

	std::shared_ptr<ITexture2D> ITexture2D::Create(const std::string& path)
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

	uint32_t ITexture::CalculateMipMapCount(uint32_t width, uint32_t height)
	{
		uint32_t levels = 1;
		while ((width | height) >> levels)
			levels++;

		return levels;
	}

}