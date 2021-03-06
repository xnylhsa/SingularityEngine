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
	if (prepared && (IsRunning()))
	{
		Graphics::VulkanContext::Get()->beginRender();
		Graphics::VulkanContext::Get()->endRender();
	}
}
