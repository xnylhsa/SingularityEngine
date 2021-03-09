#ifndef SINGULARITY_ENGINE_RENDER_RENDERER_SWAP_CHAIN
#define SINGULARITY_ENGINE_RENDER_RENDERER_SWAP_CHAIN
#include "Common.h"

namespace SingularityEngine::SERenderer
{
	class ISwapChain
	{
	public:
		virtual ~ISwapChain() = default;
		virtual void onResize(uint32_t w, uint32_t h) = 0;
		virtual void prepareFrame() = 0;
		virtual void beginScreenFrame() = 0;
		virtual void endScreenFrame() = 0;
		virtual void present() = 0;
		virtual void setScreenClearColor(Math::Color clearColor) { mScreenClearColor = clearColor; }
		virtual bool initialize() = 0;
		virtual void teardown() = 0;
		static std::unique_ptr<ISwapChain> Create();
	protected:
		Math::Color mScreenClearColor;
	};
}

#endif 
