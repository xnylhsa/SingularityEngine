#include "Precompiled.h"
#include "InputSystem.h"
using namespace SingularityEngine;
using namespace SingularityEngine::Input;

BOOL CALLBACK SingularityEngine::Input::EnumGamePadCallback(const DIDEVICEINSTANCE* pDIDeviceInstance, VOID* pContext)
{
	// Obtain an interface to the enumerated joystick
	InputSystem* inputSystem = static_cast<InputSystem*>(pContext);
	IDirectInput8* pDI = inputSystem->mDirectInput;
	IDirectInputDevice8** pGamePad = &(inputSystem->mGamePadDevice);
	if (FAILED(pDI->CreateDevice(pDIDeviceInstance->guidInstance, pGamePad, nullptr))) 
	{
		LOG("[Input::InputSystem] Failed to create game pad device.");
	}

	return DIENUM_STOP;
}

namespace
{
	void ClipToWindow(HWND window)
	{
		RECT rect;
		GetClientRect(window, &rect);

		POINT ul;
		ul.x = rect.left;
		ul.y = rect.top;

		POINT lr;
		lr.x = rect.right;
		lr.y = rect.bottom;

		MapWindowPoints(window, nullptr, &ul, 1);
		MapWindowPoints(window, nullptr, &lr, 1);

		rect.left = ul.x;
		rect.top = ul.y;

		rect.right = lr.x;
		rect.bottom = lr.y;

		ClipCursor(&rect);
	}
	InputSystem* sInputSystem = nullptr;
}

void InputSystem::StaticInitialize(HWND window, bool useRawInput)
{
	ASSERT(sInputSystem == nullptr, "[Input::InputSystem] System already initialized!");
	sInputSystem = new InputSystem();
	sInputSystem->Initialize(window, useRawInput);
}

void InputSystem::StaticTerminate()
{
	if (sInputSystem != nullptr)
	{
		sInputSystem->Terminate();
		SafeDelete(sInputSystem);
	}
}

InputSystem* SingularityEngine::Input::InputSystem::Get()
{
	ASSERT(sInputSystem != nullptr, "[Input::InputSystem] No system registered.");
	return sInputSystem;
}


//----------------------------------------------------------------------------------------------------

LRESULT InputSystem::ProcessMessage(HWND window, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (sInputSystem == nullptr)
	{
		return false;
	}

	// Capture new state
	switch (msg)
	{
	case WM_ACTIVATEAPP:
		return true;
	case WM_LBUTTONDOWN:
		sInputSystem->mCurrentMouseButtons[0] = true;
		return true;
	case WM_LBUTTONUP:
		sInputSystem->mCurrentMouseButtons[0] = false;
		return true;
	case WM_RBUTTONDOWN:
		sInputSystem->mCurrentMouseButtons[1] = true;
		return true;
	case WM_RBUTTONUP:
		sInputSystem->mCurrentMouseButtons[1] = false;
		return true;
	case WM_MBUTTONDOWN:
		sInputSystem->mCurrentMouseButtons[2] = true;
		return true;
	case WM_MBUTTONUP:
		sInputSystem->mCurrentMouseButtons[2] = false;
		return true;

	case WM_XBUTTONDOWN:
	{
		//int vkFlags = (signed short)(lParam);
		int button = (signed short)(lParam >> 16);
		if (button & XBUTTON1)
		{
			sInputSystem->mCurrentMouseButtons[3] = true;
		}
		if (button & XBUTTON2)
		{
			sInputSystem->mCurrentMouseButtons[4] = true;
		}
	}
		return true;
	case WM_XBUTTONUP:
	{
		//int vkFlags = (signed short)(lParam);
		int button = (signed short)(lParam >> 16);
		if (button & XBUTTON1)
		{
			sInputSystem->mCurrentMouseButtons[3] = false;
		}
		if (button & XBUTTON2)
		{
			sInputSystem->mCurrentMouseButtons[4] = false;
		}
	}
		return true;
	case WM_MOUSEWHEEL:
		sInputSystem->mLastMouseWheel = sInputSystem->mMouseWheel;
		sInputSystem->mMouseWheel += GET_WHEEL_DELTA_WPARAM(wParam);
		return true;
	case WM_MOUSEMOVE:
		{
			int mouseX = (signed short)(lParam);
			int mouseY = (signed short)(lParam >> 16);

			sInputSystem->mMouseScreenPositionX = mouseX;
			sInputSystem->mMouseScreenPositionY = mouseY;

			if (sInputSystem->mClipMouseToWindow)
			{
				ClipToWindow(window);
			}
			else
			{
				ClipCursor(nullptr);
			}

			RECT rect;
			GetClientRect(window, &rect);
			sInputSystem->mMouseLeftEdge = mouseX <= rect.left;
			sInputSystem->mMouseRightEdge = mouseX + 1 >= rect.right;
			sInputSystem->mMouseTopEdge = mouseY <= rect.top;
			sInputSystem->mMouseBottomEdge = mouseY + 1 >= rect.bottom;
		}
		return true;
	case WM_INPUT:
		{
			UINT dwSize = 40;
			static BYTE lpb[40];

			GetRawInputData((HRAWINPUT)lParam, RID_INPUT,
				lpb, &dwSize, sizeof(RAWINPUTHEADER));

			RAWINPUT* raw = (RAWINPUT*)lpb;
			ClipToWindow(window);
			if (raw->header.dwType == RIM_TYPEMOUSE)
			{
				sInputSystem->mMouseDeltaX = raw->data.mouse.lLastX;
				sInputSystem->mMouseDeltaY = raw->data.mouse.lLastY;
				RECT rect;
				GetClientRect(window, &rect);
				sInputSystem->mLastMouseScreenPositionX = sInputSystem->mMouseScreenPositionX;
				sInputSystem->mLastMouseScreenPositionY = sInputSystem->mMouseScreenPositionY;


				sInputSystem->mMouseScreenPositionX += sInputSystem->mMouseDeltaX;
				sInputSystem->mMouseScreenPositionY += sInputSystem->mMouseDeltaY;
				//sInputSystem->mMouseScreenPositionX = (sInputSystem->mMouseScreenPositionX < (int)rect.left) ? rect.left : ((sInputSystem->mMouseScreenPositionX > (int)rect.right) ? (int)rect.right : sInputSystem->mMouseScreenPositionX);
				//sInputSystem->mMouseScreenPositionY = (sInputSystem->mMouseScreenPositionY < (int)rect.top) ? rect.top : ((sInputSystem->mMouseScreenPositionY > (int)rect.bottom) ? (int)rect.bottom : sInputSystem->mMouseScreenPositionY);

				sInputSystem->mMouseLeftEdge = sInputSystem->mMouseScreenPositionX <= rect.left;
				sInputSystem->mMouseRightEdge = sInputSystem->mMouseScreenPositionX + 1 >= rect.right;
				sInputSystem->mMouseTopEdge = sInputSystem->mMouseScreenPositionX <= rect.top;
				sInputSystem->mMouseBottomEdge = sInputSystem->mMouseScreenPositionX + 1 >= rect.bottom;
				USHORT mouseButtonStateFlag = raw->data.mouse.usButtonFlags;

				if (mouseButtonStateFlag & RI_MOUSE_BUTTON_1_DOWN)
				{
					sInputSystem->mCurrentMouseButtons[0] = true;
				}
				if (mouseButtonStateFlag & RI_MOUSE_BUTTON_1_UP)
				{
					sInputSystem->mCurrentMouseButtons[0] = false;
				}
				if (mouseButtonStateFlag & RI_MOUSE_BUTTON_2_DOWN)
				{
					sInputSystem->mCurrentMouseButtons[1] = true;
				}
				if (mouseButtonStateFlag & RI_MOUSE_BUTTON_2_UP)
				{
					sInputSystem->mCurrentMouseButtons[1] = false;
				}
				if (mouseButtonStateFlag & RI_MOUSE_BUTTON_3_DOWN)
				{
					sInputSystem->mCurrentMouseButtons[2] = true;
				}
				if (mouseButtonStateFlag & RI_MOUSE_BUTTON_3_UP)
				{
					sInputSystem->mCurrentMouseButtons[2] = false;
				}
				if (mouseButtonStateFlag & RI_MOUSE_BUTTON_4_DOWN)
				{
					sInputSystem->mCurrentMouseButtons[3] = true;
				}
				if (mouseButtonStateFlag & RI_MOUSE_BUTTON_4_UP)
				{
					sInputSystem->mCurrentMouseButtons[3] = false;
				}
				if (mouseButtonStateFlag & RI_MOUSE_BUTTON_5_DOWN)
				{
					sInputSystem->mCurrentMouseButtons[4] = true;
				}
				if (mouseButtonStateFlag & RI_MOUSE_BUTTON_5_UP)
				{
					sInputSystem->mCurrentMouseButtons[4] = false;
				}
				if (mouseButtonStateFlag & RI_MOUSE_WHEEL)
				{
					sInputSystem->mLastMouseWheel = sInputSystem->mMouseWheel;
					sInputSystem->mMouseWheel += (short)((unsigned short)raw->data.mouse.usButtonData);
				}
			}
			if (raw->header.dwType == RIM_TYPEKEYBOARD)
			{
				USHORT vkey = raw->data.keyboard.VKey;
				UINT keyMsg = raw->data.keyboard.Message;
				if (vkey < 256 )
				{
					if (keyMsg == WM_KEYDOWN)
					{
						sInputSystem->mCurrentKeyStateBuffer[vkey] = true;
					}
					else if (keyMsg == WM_KEYUP)
					{
						sInputSystem->mCurrentKeyStateBuffer[vkey] = false;
					}

				}
				
			}
			if (sInputSystem->mLockedMouse)
			{
				int wWidth = GetSystemMetrics(SM_CXSCREEN);
				int wHeight = GetSystemMetrics(SM_CYSCREEN);
				SetCursorPos(wWidth >> 1, wHeight >> 1);
			}
		}
		return true;
	case WM_KEYDOWN:
		if (wParam < 256)
		{
			sInputSystem->mCurrentKeyStateBuffer[wParam] = true;
		}
		return true;
	case WM_KEYUP:
		if (wParam < 256)
		{
			sInputSystem->mCurrentKeyStateBuffer[wParam] = false;
		}
		return true;
	case WM_CHAR:
		//// You can also use ToAscii()+GetKeyboardState() to retrieve characters.
		//if (wParam > 0 && wParam < 0x10000)
		//	io.AddInputCharacter((unsigned short)wParam);
		return true;
	case WM_SETCURSOR:
		if (sInputSystem->mUsingRawInput)
		{
			SetCursor(NULL);
		}
		return true;
	case WM_SETFOCUS:
	{
		if (sInputSystem->mUsingRawInput)
		{
			sInputSystem->ShowSystemCursor(false);
			sInputSystem->SetMouseClipToWindow(true);
		}

	}
	return true;
	}
	return false;

}



InputSystem::InputSystem()
	: mDirectInput(nullptr)
	, mGamePadDevice(nullptr)
	, mClipMouseToWindow(false)
	, mMouseScreenPositionX(-1)
	, mMouseScreenPositionY(-1)
	, mMouseDeltaX(0)
	, mMouseDeltaY(0)
	, mMouseWheel(0)
	, mMouseLeftEdge(false)
	, mMouseRightEdge(false)
	, mMouseTopEdge(false)
	, mMouseBottomEdge(false)
	, mInitialized(false)
{
	ZeroMemory(&mCurrentKeyStateBuffer, sizeof(mCurrentKeyStateBuffer));
	ZeroMemory(&mLastKeyStateBuffer, sizeof(mLastKeyStateBuffer));
	ZeroMemory(&mPressedKeys, sizeof(mPressedKeys));


	ZeroMemory(&mCurrentMouseButtons, sizeof(mCurrentMouseButtons));
	ZeroMemory(&mLastKeyStateBuffer, sizeof(mLastKeyStateBuffer));
	ZeroMemory(&mPressedMouseButtons, sizeof(mPressedMouseButtons));


	ZeroMemory(&mCurrGamePadState, sizeof(DIJOYSTATE));
	ZeroMemory(&mPrevGamePadState, sizeof(DIJOYSTATE));
}

InputSystem::~InputSystem()
{
	ASSERT(!mInitialized, "[Input::InputSystem] Terminate() must be called to clean up!");
}
void InputSystem::CreateRawInputKeyBoardAndMouse()
{
	mRawInputDevices[0].usUsagePage = HID_USAGE_PAGE_GENERIC;
	mRawInputDevices[0].usUsage = HID_USAGE_GENERIC_MOUSE;
	mRawInputDevices[0].dwFlags = RIDEV_NOLEGACY;
	mRawInputDevices[0].hwndTarget = 0;
	mRawInputDevices[1].usUsagePage = HID_USAGE_PAGE_GENERIC;
	mRawInputDevices[1].usUsage = HID_USAGE_GENERIC_KEYBOARD;
	mRawInputDevices[1].dwFlags = RIDEV_NOLEGACY;
	mRawInputDevices[1].hwndTarget = 0;
	BOOL result = RegisterRawInputDevices(mRawInputDevices, 2, sizeof(mRawInputDevices[0]));
	if (!result)
	{
		DWORD errorMessageID = ::GetLastError();
		LPSTR messageBuffer = nullptr;
		size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);

		std::string message(messageBuffer, size);
		ASSERT(result, " [Input::InputSystem] Failed to create Raw Input Devices.");
	}
}
void InputSystem::DestroyRawInputKeyboardAndMouse()
{
	mRawInputDevices[0].usUsagePage = HID_USAGE_PAGE_GENERIC;
	mRawInputDevices[0].usUsage = HID_USAGE_GENERIC_MOUSE;
	mRawInputDevices[0].dwFlags = RIDEV_REMOVE;
	mRawInputDevices[0].hwndTarget = NULL;
	mRawInputDevices[1].usUsagePage = HID_USAGE_PAGE_GENERIC;
	mRawInputDevices[1].usUsage = HID_USAGE_GENERIC_KEYBOARD;
	mRawInputDevices[1].dwFlags = RIDEV_REMOVE;
	mRawInputDevices[1].hwndTarget = NULL;
	BOOL result = RegisterRawInputDevices(mRawInputDevices, 2, sizeof(mRawInputDevices[0]));
	ASSERT(result, "[Input::InputSystem] Failed to create Raw Input Devices.");
}
void InputSystem::Initialize(HWND window, bool useRawInput)
{
	// Check if we have already initialized the system
	if (mInitialized)
	{
		LOG("[Input::InputSystem] System already initialized.");
		return;
	}
	LOG("[Input::InputSystem] Initializing...");
	mUsingRawInput = useRawInput;
	if (mUsingRawInput)
	{
		CreateRawInputKeyBoardAndMouse();
		RECT rect;
		GetClientRect(window, &rect);
		int wWidth = rect.right - rect.left;
		int wHeight = rect.bottom - rect.top;
		mMouseScreenPositionX = wWidth >> 1;
		mMouseScreenPositionY = wHeight >> 1;
		POINT mousePos;
		mousePos.x = wWidth >> 1;
		mousePos.y = wHeight >> 1;
		ClientToScreen(window, &mousePos);

		SetCursorPos(mousePos.x, mousePos.y);
		SetMouseClipToWindow(true);
		ShowSystemCursor(false);
	}
	mWindow = window;
	// Obtain an interface to DirectInput
	HRESULT hr = DirectInput8Create(GetModuleHandle(nullptr), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&mDirectInput, nullptr);
	ASSERT(SUCCEEDED(hr), "[Input::InputSystem] Failed to create DirectInput object.");

	//----------------------------------------------------------------------------------------------------
	// Enumerate for game pad device
	if (FAILED(mDirectInput->EnumDevices(DI8DEVCLASS_GAMECTRL, EnumGamePadCallback, this, DIEDFL_ATTACHEDONLY)))
	{
		LOG("[Input::InputSystem] Failed to enumerate for game pad devices.");
	}

	// Check if we have a game pad detected
	if (mGamePadDevice != nullptr)
	{
		// Set the game pad data format
		hr = mGamePadDevice->SetDataFormat(&c_dfDIJoystick);
		ASSERT(SUCCEEDED(hr), "[Input::InputSystem] Failed to set game pad data format.");

		// Set the game pad cooperative level
		hr = mGamePadDevice->SetCooperativeLevel(window, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
		ASSERT(SUCCEEDED(hr), "[Input::InputSystem] Failed to set game pad cooperative level.");

		// Acquire the game pad device
		hr = mGamePadDevice->Acquire();
		ASSERT(SUCCEEDED(hr), "[Input::InputSystem] Failed to acquire game pad device.");
	}
	else
	{
		LOG("[Input::InputSystem] No game pad attached.");
	}

	// Set flag
	mInitialized = true;

	LOG("[Input::InputSystem] System initialized.");
}

void InputSystem::Terminate()
{
	// Check if we have already terminated the system
	if (!mInitialized)
	{
		LOG("[Input::InputSystem] System already terminated.");
		return;
	}

	LOG("[Input::InputSystem] Terminating...");
	if (mRawInputDevices)
	{
		DestroyRawInputKeyboardAndMouse();
		ShowSystemCursor(true);
	}
	// Release devices
	if (mGamePadDevice != nullptr)
	{
		mGamePadDevice->Unacquire();
		mGamePadDevice->Release();
		mGamePadDevice = nullptr;
	}

	SafeRelease(mDirectInput);

	// Set flag
	mInitialized = false;

	LOG("[Input::InputSystem] System terminated.");
}

void InputSystem::Update()
{
	ASSERT(mInitialized, "[Input::InputSystem] System not initialized.");

	// Store the previous keyboard state
	for (int i = 0; i < 512; ++i)
	{
		mPressedKeys[i] = !mLastKeyStateBuffer[i] && mCurrentKeyStateBuffer[i];
	}
	if (!mLockedMouse)
	{
		// Update mouse movement
		mMouseDeltaX = mMouseScreenPositionX - mLastMouseScreenPositionX;
		mMouseDeltaY = mMouseScreenPositionY - mLastMouseScreenPositionY;
		mLastMouseScreenPositionX = mMouseScreenPositionX;
		mLastMouseScreenPositionY = mMouseScreenPositionY;
	}
		mDeltaMouseWheel = mMouseWheel - mLastMouseWheel;
		mLastMouseWheel = mMouseWheel;
	// Store the previous mouse state
	for (int i = 0; i < 3; ++i)
	{
		mPressedMouseButtons[i] = !mLastMouseButtons[i] && mCurrentMouseButtons[i];
	}
	// Update game pad
	if (mGamePadDevice != nullptr)
	{
		UpdateGamePad();
	}
}

bool InputSystem::IsKeyDown(uint32_t key) const
{
	return mCurrentKeyStateBuffer[key];
}

bool InputSystem::IsKeyPressed(uint32_t key) const
{
	return mPressedKeys[key];
}

bool InputSystem::IsMouseDown(uint32_t button) const
{
	return mCurrentMouseButtons[button];
}

bool InputSystem::IsMousePressed(uint32_t button) const
{
	return mPressedMouseButtons[button];
}

int InputSystem::GetMouseScreenX() const
{
	return mMouseScreenPositionX;
}

int InputSystem::GetMouseScreenY() const
{
	return mMouseScreenPositionY;
}

bool InputSystem::IsMouseLeftEdge() const
{
	return mMouseLeftEdge;
}

bool InputSystem::IsMouseRightEdge() const
{
	return mMouseRightEdge;
}

bool InputSystem::IsMouseTopEdge() const
{
	return mMouseTopEdge;
}

bool InputSystem::IsMouseBottomEdge() const
{
	return mMouseBottomEdge;
}

int InputSystem::GetMouseMoveX() const
{
	return mMouseDeltaX;
}

int InputSystem::GetMouseMoveY() const
{
	return mMouseDeltaY;
}

float InputSystem::GetMouseScrollWheel() const
{
	return mDeltaMouseWheel;
}

void InputSystem::ShowSystemCursor(bool show)
{
	ShowCursor(show);
}
void InputSystem::SetLockedMouse(bool lock)
{
	mLockedMouse = lock;
}

void InputSystem::SetMouseClipToWindow(bool clip)
{
	mClipMouseToWindow = clip;
}
bool InputSystem::IsMouseClippedToWindow() const
{
	return mClipMouseToWindow;
}
bool InputSystem::UsingRawInput() const
{
	return mUsingRawInput;
}


bool InputSystem::IsGamePadButtonDown(GamePadButton button) const
{
	return (mCurrGamePadState.rgbButtons[(uint32_t)button] & 0x80) != 0;
}

bool InputSystem::IsGamePadButtonPressed(GamePadButton button) const
{
	const bool currState = (mCurrGamePadState.rgbButtons[(uint32_t)button] & 0x80) != 0;
	const bool prevState = (mPrevGamePadState.rgbButtons[(uint32_t)button] & 0x80) != 0;
	return !prevState && currState;
}

bool InputSystem::IsDPadUp() const
{
	const bool hasGamePad = (mGamePadDevice != nullptr);
	return hasGamePad && (mCurrGamePadState.rgdwPOV[0] == 0);
}

bool InputSystem::IsDPadDown() const
{
	return (mCurrGamePadState.rgdwPOV[0] == 18000);
}

bool InputSystem::IsDPadLeft() const
{
	return (mCurrGamePadState.rgdwPOV[0] == 27000);
}

bool InputSystem::IsDPadRight() const
{
	return (mCurrGamePadState.rgdwPOV[0] == 9000);
}

float InputSystem::GetLeftAnalogX() const
{
	return (mCurrGamePadState.lX / 32767.5f) - 1.0f;
}

float InputSystem::GetLeftAnalogY() const
{
	return -(mCurrGamePadState.lY / 32767.5f) + 1.0f;
}

float InputSystem::GetRightAnalogX() const
{
	return (mCurrGamePadState.lZ / 32767.5f) - 1.0f;
}

float InputSystem::GetRightAnalogY() const
{
	return -(mCurrGamePadState.lRz / 32767.5f) + 1.0f;
}

void InputSystem::UpdateGamePad()
{
	// Store the previous game pad state
	memcpy(&mPrevGamePadState, &mCurrGamePadState, sizeof(DIJOYSTATE));

	// Poll the game pad device
	static bool sWriteToLog = true;
	HRESULT hr = mGamePadDevice->Poll();
	if (FAILED(hr))
	{   
		// Check if the device is lost
		if (DIERR_INPUTLOST == hr || DIERR_NOTACQUIRED == hr)
		{
			if (sWriteToLog)
			{
				LOG("[Input::InputSystem] Game pad device is lost.");
				sWriteToLog = false;
			}

			// Try to acquire game pad device again
			mGamePadDevice->Acquire();
		}
		else
		{
			LOG("[Input::InputSystem] Failed to get game pad state.");
			return;
		}
	}
	else
	{
		// Reset flag
		sWriteToLog = true;
	}

	// Get game pad state
	hr = mGamePadDevice->GetDeviceState(sizeof(DIJOYSTATE), (void*)&mCurrGamePadState);
	if (FAILED(hr))
	{   
		// Check if the device is lost
		if (DIERR_INPUTLOST == hr || DIERR_NOTACQUIRED == hr)
		{
			if (sWriteToLog)
			{
				LOG("[Input::InputSystem] Game pad device is lost.");
				sWriteToLog = false;
			}

			// Try to acquire game pad device again
			mGamePadDevice->Acquire();
		}
		else
		{
			LOG("[Input::InputSystem] Failed to get game pad state.");
			return;
		}
	}
	else
	{
		// Reset flag
		sWriteToLog = true;
	}
}