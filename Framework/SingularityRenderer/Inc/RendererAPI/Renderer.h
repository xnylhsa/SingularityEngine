#ifndef SINGULARITY_ENGINE_RENDERAPI_RENDERER
#define SINGULARITY_ENGINE_RENDERAPI_RENDERER
#include "RenderCommand.h"

namespace SingularityEngine::SERenderer 
{
	class IRenderContext;
	class ISwapChain;
	class IGraphicsDevice;
	using RenderingCommand = std::function<void()>;

	enum class RenderingAPI
	{
		None = 0,
		Vulkan,
		DirectX12,
		APIMax
	};
	class Renderer
	{
	public:
		static void Initalize();
		static void Terminate();
		static Renderer* Get();
		static RenderingAPI GetAPI() { return s_API; }
		static RenderingAPI SetAPI(RenderingAPI api) { s_API = api; }

		ISwapChain* getSwapchain() { return mSwapChain.get(); }
		IRenderContext* getRenderContext() { return mRenderContext.get(); }
		std::shared_ptr<IGraphicsDevice> getGraphicsDevice() { return mDevice; }
		void setClearColor(Math::Color clearColor);
		void beginScene();
		void endScene();
		void resize(uint32_t width, uint32_t height);
	private:
		void onInitalize();
		void onTerminate();

		inline static Renderer* sRenderer = nullptr;
		std::unique_ptr<IRenderContext> mRenderContext;
		std::unique_ptr<ISwapChain> mSwapChain;
		std::shared_ptr<IGraphicsDevice> mDevice;
		inline static RenderingAPI s_API = RenderingAPI::Vulkan;
	};

}

#endif