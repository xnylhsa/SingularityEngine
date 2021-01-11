#include "Precompiled.h"
#include "Window.h"
#include "Debug.h"
#include "Common.h"
using namespace SingularityEngine;
using namespace SingularityEngine::Core;


LRESULT CALLBACK WinProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam)
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


Window::Window()
	: mInstance(nullptr)
	, mWindow(nullptr)
{

}

Window::~Window()
{

}

void Window::Initialize(HINSTANCE instance, LPCSTR appName, uint32_t width, uint32_t height)
{
	mInstance = instance;
	mAppName = appName;

	//Every windows window requires at least one window object. three things are involved;
	//1) Register a window class/
	//2) Create a window objective
	//3) Retrieve and dispatch for this window


	WNDCLASSEXA wcex;
	wcex.cbSize = sizeof(WNDCLASSEXA);
	wcex.style = CS_HREDRAW | CS_VREDRAW ;
	wcex.lpfnWndProc = WinProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = instance;
	wcex.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wcex.lpszMenuName = nullptr;
	wcex.lpszClassName = appName;
	wcex.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);

	VERIFY(RegisterClassExA(&wcex), "[Core::Window] Failed to register window class");
	//compute the corect window dimension
	RECT rc = { 0,0, (LONG)width, (LONG)height };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
	const int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	const int screenHeight = GetSystemMetrics(SM_CYSCREEN);
	const int winWidth = rc.right - rc.left;
	const int winHeight = rc.bottom - rc.top;
	const int left = (screenWidth - winWidth) >> 1;
	const int top = (screenHeight - winHeight) >> 1;

	mWindow = CreateWindowA
	(
		appName,
		appName,
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
void Window::Initialize(HINSTANCE instance, LPTSTR appName, uint32_t width, uint32_t height,WNDPROC wndProcFunction)
{
	mInstance = instance;
	mAppName = CT2A(appName);

	//Every windows window requires at least one window object. three things are involved;
	//1) Register a window class/
	//2) Create a window objective
	//3) Retrieve and dispatch for this window


	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEXA);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = wndProcFunction;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = instance;
	wcex.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wcex.lpszMenuName = nullptr;
	wcex.lpszClassName = appName;
	wcex.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);

	VERIFY(RegisterClassEx(&wcex), "[Core::Window] Failed to register window class");
	//compute the corect window dimension
	RECT rc = { 0,0, (LONG)width, (LONG)height };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
	const int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	const int screenHeight = GetSystemMetrics(SM_CYSCREEN);
	const int winWidth = rc.right - rc.left;
	const int winHeight = rc.bottom - rc.top;
	const int left = (screenWidth - winWidth) >> 1;
	const int top = (screenHeight - winHeight) >> 1;

	mWindow = CreateWindow
	(
		appName,
		appName,
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
void Window::Terminate()
{
	if (mWindow)
	{
		DestroyWindow(mWindow);
		VERIFY(UnregisterClassA(mAppName.c_str(), mInstance), "[Core::Window] Failed to unregister window class.");
		mWindow = nullptr;
		mInstance = nullptr;
	}
}
bool Window::ProcessMessage()
{
	MSG msg = { 0 };
	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return (WM_QUIT == msg.message);
}