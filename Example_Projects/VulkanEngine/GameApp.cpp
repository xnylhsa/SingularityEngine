#include "GameApp.h"
using namespace SingularityEngine;
bool resizing = false;
bool prepared = false;

GameApp::GameApp()
{

}

GameApp::~GameApp()
{

}

void GameApp::onInitialize()
{
	registerEventFunc(Core::EventType::WindowResize, BIND_EVENT_FN(GameApp::onResize));
	SERenderer::Renderer::Initalize();
	SERenderer::Renderer::Get()->setClearColor(Math::Color(1.0f, 0.0f, 1.0f, 1.0f));
	prepared = true;
}

void GameApp::onTerminate()
{
	SERenderer::Renderer::Terminate();
}

bool GameApp::onResize(Core::Event& e)
{
	prepared = false;
	if (isFocused())
	{
		Core::WindowResizeEvent& event = *dynamic_cast<Core::WindowResizeEvent*>(&e);
		SERenderer::Renderer::Get()->resize((uint32_t)event.getWidth(), (uint32_t)event.getHeight());
		prepared = true;
	}
	return true;
}

void GameApp::onUpdate()
{

	if (prepared && isFocused() && isRunning())
	{

		SERenderer::Renderer::Get()->beginScene();
		SERenderer::Renderer::Get()->endScene();
	}
}
