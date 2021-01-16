#include "GameApp.h"

using namespace SingularityEngine;

LRESULT WINAPI MyWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{

	Input::InputSystem::ProcessMessage(hWnd, msg, wParam, lParam);
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_SIZE:
		UINT width = LOWORD(lParam);
		UINT height = HIWORD(lParam);
		if (SingularityEngine::Vulkan::GraphicsSystem::isInitalized())
		{
			SingularityEngine::Vulkan::GraphicsSystem* gs = SingularityEngine::Vulkan::GraphicsSystem::Get();
			gs->onWindowSizeChanged(width, height);
		}
		return 0;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

GameApp::GameApp() : wWidth(0), wHeight(0)
{

}

GameApp::~GameApp()
{

}

void GameApp::OnInitialize(uint32_t w, uint32_t h)
{
	wWidth = w;
	wHeight = h;
	std::string appNamestring = GetAppName();
	std::wstring appName(appNamestring.length(), L' '); // Make room for characters
	std::copy(appNamestring.begin(), appNamestring.end(), appName.begin());
	mWindow.Initialize(GetInstance(), (LPTSTR)appName.c_str(), wWidth, wHeight, MyWndProc);
	Input::InputSystem::StaticInitialize(mWindow.GetWindowHandle(), false);


	Vulkan::StartupParameters startupInfo;
	startupInfo.mDeviceParameters.mDesiredDeviceExtensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
	startupInfo.mInstanceParameters.mDesiredExtensions.push_back(VK_KHR_SURFACE_EXTENSION_NAME);
#if defined(_WIN32) || (_WIN64)
	startupInfo.mInstanceParameters.mDesiredExtensions.push_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
#elif defined(VK_USE_PLATFORM_XCB_KHR)
	startupInfo.mInstanceParameters.mDesiredExtensions.push_back(VK_KHR_XCB_SURFACE_EXTENSION_NAME);
#elif defined(VK_USE_PLATFORM_XLIB_KHR)
	startupInfo.mInstanceParameters.mDesiredExtensions.push_back(VK_KHR_XLIB_SURFACE_EXTENSION_NAME);
#endif
	startupInfo.mDeviceParameters.mDesiredFeatures.geometryShader = VK_TRUE;

	startupInfo.mAppName = appNamestring;
	startupInfo.mMajorVersion = 0;
	startupInfo.mMinorVersion = 1;
	startupInfo.mPatchVersion = 1;

	startupInfo.mEngineName = "Singularity Engine";
	startupInfo.mEngineMajorVersion = 0;
	startupInfo.mEngineMinorVersion = 1;
	startupInfo.mEnginePatchVersion = 1;

	startupInfo.windowParams.mAppInstance = GetInstance();
	startupInfo.windowParams.mWindowHandle = mWindow.GetWindowHandle();
	startupInfo.windowParams.width = w;
	startupInfo.windowParams.width = h;

	Vulkan::GraphicsSystem::StaticInitialize(startupInfo);
}

void GameApp::OnTerminate()
{
	Vulkan::GraphicsSystem::StaticTerminate();
}

void GameApp::OnUpdate()
{
	if (mWindow.ProcessMessage())
	{
		Kill();
		return;
	}
	auto input = Input::InputSystem::Get();
	input->Update();
	if (input->IsKeyDown((uint32_t)Input::Keys::ESCAPE))
	{
		PostQuitMessage(0);
	}
}
