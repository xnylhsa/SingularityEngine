#include "Precompiled.h"
#include "RendererAPI/Renderer.h"
#include "RendererAPI/ISwapChain.h"
#include "RendererAPI/RendererContext.h"
#include "RendererAPI/IGraphicsDevice.h"
namespace SingularityEngine::SERenderer
{
	void Renderer::beginScene()
	{
		mSwapChain->prepareFrame();
		mSwapChain->beginScreenFrame();
	}

	void Renderer::endScene()
	{
		mSwapChain->endScreenFrame();
		mSwapChain->present();
	}

	void Renderer::Initalize()
	{
		ASSERT(!sRenderer, "[Renderer] initalizer called twice!");
		if (sRenderer) return;
		sRenderer = new Renderer();
		sRenderer->onInitalize();
	}

	void Renderer::Terminate()
	{
		sRenderer->onTerminate();
		delete sRenderer;
		sRenderer = nullptr;
	}

	void Renderer::resize(uint32_t width, uint32_t height)
	{
		mSwapChain->onResize(width, height);
	}

	Renderer* Renderer::Get()
	{
		ASSERT(sRenderer, "[Renderer] Failed To initalize Renderer!");
		return sRenderer;
	}

	void Renderer::onInitalize()
	{
		mRenderContext = std::move(IRenderContext::Create());
		mRenderContext->initialize();
		mDevice = std::move(IGraphicsDevice::Create());
		mDevice->initialize();
		mSwapChain = std::move(ISwapChain::Create());
		mSwapChain->initialize();
	}

	void Renderer::onTerminate()
	{
		mDevice->waitUntilIdle();
		mSwapChain->teardown();
		mSwapChain.reset();
		mDevice->teardown();
		mDevice.reset();
		mRenderContext->teardown();
		mRenderContext.reset();
	}

	void Renderer::setClearColor(Math::Color clearColor)
	{
		mSwapChain->setScreenClearColor(clearColor);
	}

}