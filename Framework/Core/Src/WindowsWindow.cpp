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
		auto* window = static_cast<Core::WindowsWindow*>(GetPropW(handle, L"SINGULARITY"));
		if (!window)
		{
			return DefWindowProcA(handle, message, wParam, lParam);
		}
		switch (message)
		{
		case WM_SETFOCUS:
		{
			WindowFocusEvent focusEvent;
			window->propigateEvent(focusEvent);
			return 0;
		}
		case WM_KILLFOCUS:
		{
			WindowLostFocusEvent focusEvent;
			window->propigateEvent(focusEvent);
			return 0;
		}
		case WM_CLOSE:
		{
			WindowCloseEvent closeEvent;
			window->propigateEvent(closeEvent);
			return 0;
		}
		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
			if (wParam < 256)
			{
				if (window->useRawInput()) { break; }
				LPARAM params = lParam;
				params >>= 15;
	

				bool isExtended = (bool)((lParam >> 24) & 1);
				UINT scanCode = MapVirtualKeyA((UINT)wParam, MAPVK_VK_TO_VSC);
				if (isExtended)
				{
					UINT first = 0xE0;
					scanCode = (first << 8) | scanCode;
				}
				SingularityInputType inputType = window->convertInput((int)scanCode);
				KeyPressedEvent pressedEvent(inputType);
				window->propigateEvent(pressedEvent);
				return true;
			}
			break;
		case WM_KEYUP:
		case WM_SYSKEYUP:
			if (wParam < 256)
			{
				if (window->useRawInput()) { break; }
				bool isExtended = (bool)((lParam >> 24) & 1);
				UINT scanCode = MapVirtualKeyA((UINT)wParam, MAPVK_VK_TO_VSC);
				if (isExtended)
				{
					UINT first = 0xE0;
					scanCode = (first << 8) | scanCode;
				}
				SingularityInputType inputType = window->convertInput((int)scanCode);
				KeyReleasedEvent pressedEvent(inputType);
				window->propigateEvent(pressedEvent);
				return true;
			}
			break;
		case WM_LBUTTONDOWN:
		{
			if (window->useRawInput()) { break; }

			MousePressedEvent pressedEvent(SingularityInputType::SEMouseLeft);
			window->propigateEvent(pressedEvent);
			return true;
		}
		case WM_LBUTTONUP:
		{
			if (window->useRawInput()) break;

			MouseReleasedEvent pressedEvent(SingularityInputType::SEMouseLeft);
			window->propigateEvent(pressedEvent);
			return true;
		}
		case WM_RBUTTONDOWN:
		{
			if (window->useRawInput()) break;

			MousePressedEvent pressedEvent(SingularityInputType::SEMouseRight);
			window->propigateEvent(pressedEvent);
			return true;
		}
		case WM_RBUTTONUP:
		{
			if (window->useRawInput()) break;

			MouseReleasedEvent pressedEvent(SingularityInputType::SEMouseRight);
			window->propigateEvent(pressedEvent);
			return true;
		}
		case WM_MBUTTONDOWN:
		{
			if (window->useRawInput()) break;

			MousePressedEvent pressedEvent(SingularityInputType::SEMouseMiddle);
			window->propigateEvent(pressedEvent);
			return true;
		}
		case WM_MBUTTONUP:
		{
			if (window->useRawInput()) break;

			MouseReleasedEvent pressedEvent(SingularityInputType::SEMouseMiddle);
			window->propigateEvent(pressedEvent);
			return true;
		}
		case WM_XBUTTONDOWN:
		{
			if (window->useRawInput()) break;

			//int vkFlags = (signed short)(lParam);
			int button = (signed short)(lParam >> 16);
			if (button & XBUTTON1)
			{
				MousePressedEvent pressedEvent(SingularityInputType::SEMouseButton4);
				window->propigateEvent(pressedEvent);
			}
			if (button & XBUTTON2)
			{
				MousePressedEvent pressedEvent(SingularityInputType::SEMouseButton5);
				window->propigateEvent(pressedEvent);
			}
			return true;
		}
		case WM_XBUTTONUP:
		{
			if (window->useRawInput()) break;

			//int vkFlags = (signed short)(lParam);
			int button = (signed short)(lParam >> 16);
			if (button & XBUTTON1)
			{
				MouseReleasedEvent pressedEvent(SingularityInputType::SEMouseButton4);
				window->propigateEvent(pressedEvent);
			}
			if (button & XBUTTON2)
			{
				MouseReleasedEvent pressedEvent(SingularityInputType::SEMouseButton5);
				window->propigateEvent(pressedEvent);
			}
			return true;
		}
		case WM_MOUSEWHEEL:
		{
			if (window->useRawInput()) break;

			WORD fwKeys = GET_KEYSTATE_WPARAM(wParam);
			if ((fwKeys & MK_SHIFT) == 0)
			{
				MouseScrolledEvent scrolledEvent(0.0f, (float)GET_WHEEL_DELTA_WPARAM(wParam));
				window->propigateEvent(scrolledEvent);
			}
			else
			{
				MouseScrolledEvent scrolledEvent((float)GET_WHEEL_DELTA_WPARAM(wParam), 0.0f);
				window->propigateEvent(scrolledEvent);
			}
			return true;
		}
		case WM_MOUSEMOVE:
		{
			if (window->useRawInput()) break;

			int mouseX = (signed short)(lParam);
			int mouseY = (signed short)(lParam >> 16);
			MouseMovedEvent movedEvent((float)mouseX, (float)mouseY);
			window->propigateEvent(movedEvent);

			if (window->clipCursorToWindow())
			{
				RECT rect;
				GetClientRect(handle, &rect);

				POINT ul;
				ul.x = rect.left;
				ul.y = rect.top;

				POINT lr;
				lr.x = rect.right;
				lr.y = rect.bottom;

				MapWindowPoints(handle, nullptr, &ul, 1);
				MapWindowPoints(handle, nullptr, &lr, 1);

				rect.left = ul.x;
				rect.top = ul.y;

				rect.right = lr.x;
				rect.bottom = lr.y;

				ClipCursor(&rect);
			}
			else
			{
				ClipCursor(nullptr);
			}
		}
		return true;
		case WM_INPUT:
		{
			if (!window->useRawInput()) break;
			UINT dwSize = 40;
			static BYTE lpb[40];

			GetRawInputData((HRAWINPUT)lParam, RID_INPUT,
				lpb, &dwSize, sizeof(RAWINPUTHEADER));

			RAWINPUT* raw = (RAWINPUT*)lpb;
			RECT rect;
			GetClientRect(handle, &rect);

			POINT ul;
			ul.x = rect.left;
			ul.y = rect.top;

			POINT lr;
			lr.x = rect.right;
			lr.y = rect.bottom;

			MapWindowPoints(handle, nullptr, &ul, 1);
			MapWindowPoints(handle, nullptr, &lr, 1);

			rect.left = ul.x;
			rect.top = ul.y;

			rect.right = lr.x;
			rect.bottom = lr.y;

			ClipCursor(&rect);

			if (raw->header.dwType == RIM_TYPEMOUSE)
			{
				MouseMovedEvent movedEvent((float)raw->data.mouse.lLastX, (float)raw->data.mouse.lLastY);
				window->propigateEvent(movedEvent);

				USHORT mouseButtonStateFlag = raw->data.mouse.usButtonFlags;

				if (mouseButtonStateFlag & RI_MOUSE_BUTTON_1_DOWN)
				{
					MousePressedEvent pressedEvent(SingularityInputType::SEMouseLeft);
					window->propigateEvent(pressedEvent);
				}
				if (mouseButtonStateFlag & RI_MOUSE_BUTTON_1_UP)
				{
					MouseReleasedEvent pressedEvent(SingularityInputType::SEMouseLeft);
					window->propigateEvent(pressedEvent);
				}
				if (mouseButtonStateFlag & RI_MOUSE_BUTTON_2_DOWN)
				{
					MousePressedEvent pressedEvent(SingularityInputType::SEMouseRight);
					window->propigateEvent(pressedEvent);
				}
				if (mouseButtonStateFlag & RI_MOUSE_BUTTON_2_UP)
				{
					MouseReleasedEvent pressedEvent(SingularityInputType::SEMouseRight);
					window->propigateEvent(pressedEvent);
				}
				if (mouseButtonStateFlag & RI_MOUSE_BUTTON_3_DOWN)
				{
					MousePressedEvent pressedEvent(SingularityInputType::SEMouseMiddle);
					window->propigateEvent(pressedEvent);
				}
				if (mouseButtonStateFlag & RI_MOUSE_BUTTON_3_UP)
				{
					MouseReleasedEvent pressedEvent(SingularityInputType::SEMouseMiddle);
					window->propigateEvent(pressedEvent);
				}
				if (mouseButtonStateFlag & RI_MOUSE_BUTTON_4_DOWN)
				{
					MousePressedEvent pressedEvent(SingularityInputType::SEMouseButton4);
					window->propigateEvent(pressedEvent);
				}
				if (mouseButtonStateFlag & RI_MOUSE_BUTTON_4_UP)
				{
					MouseReleasedEvent pressedEvent(SingularityInputType::SEMouseButton4);
					window->propigateEvent(pressedEvent);
				}
				if (mouseButtonStateFlag & RI_MOUSE_BUTTON_5_DOWN)
				{
					MousePressedEvent pressedEvent(SingularityInputType::SEMouseButton5);
					window->propigateEvent(pressedEvent);
				}
				if (mouseButtonStateFlag & RI_MOUSE_BUTTON_5_UP)
				{
					MouseReleasedEvent pressedEvent(SingularityInputType::SEMouseButton5);
					window->propigateEvent(pressedEvent);
				}
				if (mouseButtonStateFlag & RI_MOUSE_WHEEL)
				{
					MouseScrolledEvent scrolledEvent(0.0f, (float)raw->data.mouse.usButtonData);
					window->propigateEvent(scrolledEvent);
				}
			}
			if (raw->header.dwType == RIM_TYPEKEYBOARD)
			{
				USHORT vkey = raw->data.keyboard.VKey;
				UINT scanCode = MapVirtualKeyA(raw->data.keyboard.VKey, MAPVK_VK_TO_VSC);
				bool isExtended = raw->data.keyboard.Flags & RI_KEY_E0;
				if (isExtended)
				{
					UINT first = 0xE0;
					scanCode = (first << 8) | scanCode;
				}
				UINT keyMsg = raw->data.keyboard.Message;
				if (vkey < 256)
				{
					if (keyMsg == WM_KEYDOWN)
					{
						SingularityInputType inputType = window->convertInput((int)scanCode);
						KeyPressedEvent pressedEvent(inputType);
						window->propigateEvent(pressedEvent);
					}
					else if (keyMsg == WM_KEYUP)
					{
						SingularityInputType inputType = window->convertInput((int)scanCode);
						KeyReleasedEvent pressedEvent(inputType);
						window->propigateEvent(pressedEvent);
					}

				}

			}
			if (window->isCursorLocked())
			{
				int wWidth = GetSystemMetrics(SM_CXSCREEN);
				int wHeight = GetSystemMetrics(SM_CYSCREEN);
				SetCursorPos(wWidth >> 1, wHeight >> 1);
			}
		}
		return true;
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
	mInputConverter.initialize();

	//Every windows window requires at least one window object. three things are involved;
	//1) Register a window class/
	//2) Create a window objective
	//3) Retrieve and dispatch for this window
	auto wName = string_cast<std::wstring, std::string>(mAppName);

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
	VERIFY(SetCursorPos(screenWidth >> 1, screenHeight >> 1), "[Core::Window] Failed to Set Cursor Position");
	SetPropW(mWindow, L"SINGULARITY", this);
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
	ProcessMessage();
}

void WindowsWindow::propigateEvent(Event& event)
{
	if (mEventCallback)
	{
		mEventCallback(event);
		return;
	}
	ASSERT(false, "[Core::Window] no event callback registered!");
}

bool WindowsWindow::isCursorLocked()
{
	return mIsCursorLocked;
}

void WindowsWindow::setCursorLocked(bool isLocked)
{
	mIsCursorLocked = isLocked;
}

bool WindowsWindow::clipCursorToWindow()
{
	return mShouldClipCursor;
}

void WindowsWindow::setClipCursor(bool isClipped)
{
	mShouldClipCursor = isClipped;
}

bool WindowsWindow::useRawInput()
{
	return mUseRawInput;
}

void WindowsWindow::setUseRawInput(bool useRawInput)
{
	mUseRawInput = useRawInput;
}
