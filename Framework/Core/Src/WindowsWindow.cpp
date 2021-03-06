#include "Precompiled.h"
#include "WindowsWindow.h"
#include "Debug.h"
#include "Common.h"
#include "StringCasting.h"
using namespace SingularityEngine;
using namespace SingularityEngine::Core;

namespace
{
	LRESULT CALLBACK defaultWinProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam)
	{
		switch (message)
		{
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}
		}
		return DefWindowProcA(handle, message, wParam, lParam);
	}
}



WindowsWindow::WindowsWindow(const WindowProperties& props)
	: mAppName(props.mTitle)
	, mHeight(props.mHeight)
	, mWidth(props.mWidth)
	, mIsVSync(props.useVync)
	, mInstance(nullptr)
	, mWindow(nullptr)
{

	Initialize(GetModuleHandle(NULL));
}

WindowsWindow::~WindowsWindow()
{
	Terminate();
}

void WindowsWindow::Initialize(HINSTANCE instance)
{
	Initialize(instance, defaultWinProc);
}
void WindowsWindow::Initialize(HINSTANCE instance, WNDPROC wndProcFunction)
{
	HRESULT result = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
	if (result != S_OK)
	{
		return;
	}
	mInstance = instance;

	//Every windows window requires at least one window object. three things are involved;
	//1) Register a window class/
	//2) Create a window objective
	//3) Retrieve and dispatch for this window
	std::wstring wName = string_cast<std::wstring, std::string>(mAppName);

	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = wndProcFunction;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = instance;
	wcex.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wcex.lpszMenuName = nullptr;
	
	wcex.lpszClassName = wName.c_str();
	wcex.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);

	VERIFY(RegisterClassEx(&wcex), "[Core::Window] Failed to register window class");
	//compute the correct window dimension
	RECT rc = { 0,0, (LONG)mWidth, (LONG)mHeight };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
	const int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	const int screenHeight = GetSystemMetrics(SM_CYSCREEN);
	const int winWidth = rc.right - rc.left;
	const int winHeight = rc.bottom - rc.top;
	const int left = (screenWidth - winWidth) >> 1;
	const int top = (screenHeight - winHeight) >> 1;

	mWindow = CreateWindow
	(
		wName.c_str(),
		wName.c_str(),
		WS_OVERLAPPEDWINDOW,
		left,
		top,
		winWidth,
		winHeight,
		nullptr,
		nullptr,
		instance,
		nullptr
	);
	ASSERT(mWindow != nullptr, "[Core::Window] Failed to create window.");
	ShowWindow(mWindow, true);
	VERIFY(SetCursorPos(screenWidth >> 1, screenHeight >> 1), "[Core::Widnow] Failed to Set Cursor Position");

}
void WindowsWindow::Terminate()
{
	if (mWindow)
	{
		DestroyWindow(mWindow);
		VERIFY(UnregisterClassA(mAppName.c_str(), mInstance), "[Core::Window] Failed to unregister window class.");
		mWindow = nullptr;
		mInstance = nullptr;
	}
	CoUninitialize();
}
bool WindowsWindow::ProcessMessage()
{
	MSG msg = { 0 };
	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return (WM_QUIT == msg.message);
}

bool WindowsWindow::isMinimized()
{
	return IsIconic(mWindow);
}

void WindowsWindow::onUpdate()
{

}