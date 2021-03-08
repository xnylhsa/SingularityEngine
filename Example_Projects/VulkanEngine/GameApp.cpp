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
	SingularityEngine::Graphics::StartupParameters startupInfo;
	Core::Window* window = getWindow();
	startupInfo.mAppName = window->getTitle();
	startupInfo.mMajorVersion = 0;
	startupInfo.mMinorVersion = 1;
	startupInfo.mPatchVersion = 4;

	startupInfo.mWindow = window;
	Graphics::VulkanContext::StaticInitialize(startupInfo);
	prepared = true;
}

void GameApp::onTerminate()
{
	Graphics::VulkanContext::StaticTerminate();
}

bool GameApp::onResize(Core::Event& e)
{
	prepared = false;
	if (isFocused())
	{
		Core::WindowResizeEvent& event = *dynamic_cast<Core::WindowResizeEvent*>(&e);
		Graphics::VulkanContext::Get()->onWindowSizeChanged((uint32_t)event.getWidth(), (uint32_t)event.getHeight());
		prepared = true;
	}
	return true;
}

void GameApp::onUpdate()
{
	if (Core::InputManager::isKeyPressed(Core::KeyboardInputType::SEKey_Escape))
	{
		kill();
		return;
	}

	if (prepared && isFocused() && isRunning())
	{
		Graphics::VulkanContext::Get()->beginRender();
		Graphics::VulkanContext::Get()->endRender();
	}
}
