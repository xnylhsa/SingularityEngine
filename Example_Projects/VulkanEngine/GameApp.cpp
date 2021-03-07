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

void GameApp::OnInitialize()
{
	//Input::InputSystem::StaticInitialize(getWindow().GetWindowHandle(), false);

	RegisterEventFunc(Core::EventType::WindowResize, BIND_EVENT_FN(GameApp::OnResize));
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

void GameApp::OnTerminate()
{
	Graphics::VulkanContext::StaticTerminate();
}

bool GameApp::OnResize(Core::Event& e)
{
	if (IsFocused())
	{
		Core::WindowResizeEvent& event = *dynamic_cast<Core::WindowResizeEvent*>(&e);
		Graphics::VulkanContext::Get()->onWindowSizeChanged((uint32_t)event.getWidth(), (uint32_t)event.getHeight());
	}
	return true;
}

void GameApp::OnUpdate()
{
	//if (getWindow().ProcessMessage())
	//{
	//	Kill();
	//	return;
	//}
	//auto input = Input::InputSystem::Get();
	//input->Update();
	//if (input->IsKeyDown((uint32_t)Input::Keys::ESCAPE))
	//{
	//	PostQuitMessage(0);
	//	return;
	//}
	if (prepared && IsFocused() && IsRunning())
	{
		Graphics::VulkanContext::Get()->beginRender();
		Graphics::VulkanContext::Get()->endRender();
	}
}
