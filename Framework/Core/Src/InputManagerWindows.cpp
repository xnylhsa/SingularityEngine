#include "precompiled.h"
#include "InputManagerWindows.h"
#include "Debug.h"
// DirectX headers

#include "application.h"
#include "DeleteUtil.h"
#include "KeyEvent.h"
#include "MouseEvent.h"
#include "GamepadEvent.h"
namespace SingularityEngine::Core
{
#ifndef SINGULARITY_USE_GLFW
#ifdef SINGULARITY_PLATFORM_WINDOWS
	InputManager* InputManager::sInstance = new InputManagerWindows();
#endif
#endif
	BOOL CALLBACK InputManagerWindows::EnumGamePadCallback(const DIDEVICEINSTANCE* pDIDeviceInstance, VOID* pContext)
	{
		UNREFERENCED_PARAMETER(pContext);
		auto* inputManager = dynamic_cast<InputManagerWindows*>(InputManager::get());
		// Obtain an interface to the enumerated joystick
		IDirectInput8* pDI = inputManager->mDirectInput;
		IDirectInputDevice8** pGamePad = &(inputManager->mGamePadDevice);
		if (FAILED(pDI->CreateDevice(pDIDeviceInstance->guidInstance, pGamePad, nullptr)))
		{
			LOG("[Input::InputSystem] Failed to create game pad device.");
		}

		return DIENUM_STOP;
	}

	void InputManagerWindows::CreateRawInputKeyBoardAndMouse()
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

	void InputManagerWindows::DestroyRawInputKeyboardAndMouse()
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

	void InputManagerWindows::registerKeyMappings()
	{
		mKeyboardInputConverter.emplace(0x01, KeyboardInputType::SEKey_Escape);
		mKeyboardInputConverter.emplace(0x3B, KeyboardInputType::SEKey_F1);
		mKeyboardInputConverter.emplace(0x3C, KeyboardInputType::SEKey_F2);
		mKeyboardInputConverter.emplace(0x3D, KeyboardInputType::SEKey_F3);
		mKeyboardInputConverter.emplace(0x3E, KeyboardInputType::SEKey_F4);
		mKeyboardInputConverter.emplace(0x3F, KeyboardInputType::SEKey_F5);
		mKeyboardInputConverter.emplace(0x40, KeyboardInputType::SEKey_F6);
		mKeyboardInputConverter.emplace(0x41, KeyboardInputType::SEKey_F7);
		mKeyboardInputConverter.emplace(0x42, KeyboardInputType::SEKey_F8);
		mKeyboardInputConverter.emplace(0x43, KeyboardInputType::SEKey_F9);
		mKeyboardInputConverter.emplace(0x44, KeyboardInputType::SEKey_F10);
		mKeyboardInputConverter.emplace(0x57, KeyboardInputType::SEKey_F11);
		mKeyboardInputConverter.emplace(0x58, KeyboardInputType::SEKey_F12);

		mKeyboardInputConverter.emplace(0x29, KeyboardInputType::SEKey_Grave);
		mKeyboardInputConverter.emplace(0x0C, KeyboardInputType::SEKey_Minus);
		mKeyboardInputConverter.emplace(0x0D, KeyboardInputType::SEKey_Equals);
		mKeyboardInputConverter.emplace(0x0E, KeyboardInputType::SEKey_Backspace);
		mKeyboardInputConverter.emplace(0x1A, KeyboardInputType::SEKey_LeftBracket);
		mKeyboardInputConverter.emplace(0x1B, KeyboardInputType::SEKey_RightBracket);
		mKeyboardInputConverter.emplace(0x27, KeyboardInputType::SEKey_SemiColon);
		mKeyboardInputConverter.emplace(0x28, KeyboardInputType::SEKey_Apostrophe);
		mKeyboardInputConverter.emplace(0x1C, KeyboardInputType::SEKey_Enter);
		mKeyboardInputConverter.emplace(0x33, KeyboardInputType::SEKey_Comma);
		mKeyboardInputConverter.emplace(0x34, KeyboardInputType::SEKey_Period);
		mKeyboardInputConverter.emplace(0x35, KeyboardInputType::SEKey_ForwardSlash);
		mKeyboardInputConverter.emplace(0x3A, KeyboardInputType::SEKey_CapsLock);
		mKeyboardInputConverter.emplace(0x45, KeyboardInputType::SEKey_NumLock);
		mKeyboardInputConverter.emplace(0x46, KeyboardInputType::SEKey_ScrollLock);
		mKeyboardInputConverter.emplace(0xE052, KeyboardInputType::SEKey_Insert);
		mKeyboardInputConverter.emplace(0xE053, KeyboardInputType::SEKey_Del);
		mKeyboardInputConverter.emplace(0xE047, KeyboardInputType::SEKey_Home);
		mKeyboardInputConverter.emplace(0xE04F, KeyboardInputType::SEKey_End);
		mKeyboardInputConverter.emplace(0xE049, KeyboardInputType::SEKey_PageUp);
		mKeyboardInputConverter.emplace(0xE051, KeyboardInputType::SEKey_PageDown);

		mKeyboardInputConverter.emplace(0x2A, KeyboardInputType::SEKey_LeftShift);
		mKeyboardInputConverter.emplace(0x36, KeyboardInputType::SEKey_RightShift);
		mKeyboardInputConverter.emplace(0x1D, KeyboardInputType::SEKey_LeftCtrl);
		mKeyboardInputConverter.emplace(0xE01D, KeyboardInputType::SEKey_RightCtrl);
		mKeyboardInputConverter.emplace(0x38, KeyboardInputType::SEKey_LeftAlt);
		mKeyboardInputConverter.emplace(0xE038, KeyboardInputType::SEKEY_RightAlt);
		mKeyboardInputConverter.emplace(0x39, KeyboardInputType::SEKey_Space);

		mKeyboardInputConverter.emplace(0xE048, KeyboardInputType::SEKey_Up);
		mKeyboardInputConverter.emplace(0xE050, KeyboardInputType::SEKey_Down);
		mKeyboardInputConverter.emplace(0xE04B, KeyboardInputType::SEKey_Left);
		mKeyboardInputConverter.emplace(0xE04D, KeyboardInputType::SEKey_Right);

		mKeyboardInputConverter.emplace(0x52, KeyboardInputType::SENumpad_0);
		mKeyboardInputConverter.emplace(0x4F, KeyboardInputType::SENumpad_1);
		mKeyboardInputConverter.emplace(0x50, KeyboardInputType::SENumpad_2);
		mKeyboardInputConverter.emplace(0x51, KeyboardInputType::SENumpad_3);
		mKeyboardInputConverter.emplace(0x4B, KeyboardInputType::SENumpad_4);
		mKeyboardInputConverter.emplace(0x4C, KeyboardInputType::SENumpad_5);
		mKeyboardInputConverter.emplace(0x4D, KeyboardInputType::SENumpad_6);
		mKeyboardInputConverter.emplace(0x47, KeyboardInputType::SENumpad_7);
		mKeyboardInputConverter.emplace(0x48, KeyboardInputType::SENumpad_8);
		mKeyboardInputConverter.emplace(0x49, KeyboardInputType::SENumpad_9);
		mKeyboardInputConverter.emplace(0x4E, KeyboardInputType::SENumpad_Add);
		mKeyboardInputConverter.emplace(0x4A, KeyboardInputType::SENumpad_Subtract);
		mKeyboardInputConverter.emplace(0x37, KeyboardInputType::SENumpad_Multiply);
		mKeyboardInputConverter.emplace(0xE035, KeyboardInputType::SENumpad_Divide);
		mKeyboardInputConverter.emplace(0xE01C, KeyboardInputType::SENumpad_Enter);
		mKeyboardInputConverter.emplace(0x53, KeyboardInputType::SENumpad_Decimal);
		mKeyboardInputConverter.emplace(0x0F, KeyboardInputType::SEKey_Tab);
		mKeyboardInputConverter.emplace(0x2B, KeyboardInputType::SEKey_BackSlash);

		mKeyboardInputConverter.emplace(0x0B, KeyboardInputType::SEKey_0);
		mKeyboardInputConverter.emplace(0x02, KeyboardInputType::SEKey_1);
		mKeyboardInputConverter.emplace(0x03, KeyboardInputType::SEKey_2);
		mKeyboardInputConverter.emplace(0x04, KeyboardInputType::SEKey_3);
		mKeyboardInputConverter.emplace(0x05, KeyboardInputType::SEKey_4);
		mKeyboardInputConverter.emplace(0x06, KeyboardInputType::SEKey_5);
		mKeyboardInputConverter.emplace(0x07, KeyboardInputType::SEKey_6);
		mKeyboardInputConverter.emplace(0x08, KeyboardInputType::SEKey_7);
		mKeyboardInputConverter.emplace(0x09, KeyboardInputType::SEKey_8);
		mKeyboardInputConverter.emplace(0x0A, KeyboardInputType::SEKey_9);


		mKeyboardInputConverter.emplace(0x10, KeyboardInputType::SEKey_Q);
		mKeyboardInputConverter.emplace(0x11, KeyboardInputType::SEKey_W);
		mKeyboardInputConverter.emplace(0x12, KeyboardInputType::SEKey_E);
		mKeyboardInputConverter.emplace(0x13, KeyboardInputType::SEKey_R);
		mKeyboardInputConverter.emplace(0x14, KeyboardInputType::SEKey_T);
		mKeyboardInputConverter.emplace(0x15, KeyboardInputType::SEKey_Y);
		mKeyboardInputConverter.emplace(0x16, KeyboardInputType::SEKey_U);
		mKeyboardInputConverter.emplace(0x17, KeyboardInputType::SEKey_I);
		mKeyboardInputConverter.emplace(0x18, KeyboardInputType::SEKey_O);
		mKeyboardInputConverter.emplace(0x19, KeyboardInputType::SEKey_P);
		mKeyboardInputConverter.emplace(0x1E, KeyboardInputType::SEKey_A);
		mKeyboardInputConverter.emplace(0x1F, KeyboardInputType::SEKey_S);
		mKeyboardInputConverter.emplace(0x20, KeyboardInputType::SEKey_D);
		mKeyboardInputConverter.emplace(0x21, KeyboardInputType::SEKey_F);
		mKeyboardInputConverter.emplace(0x22, KeyboardInputType::SEKey_G);
		mKeyboardInputConverter.emplace(0x23, KeyboardInputType::SEKey_H);
		mKeyboardInputConverter.emplace(0x24, KeyboardInputType::SEKey_J);
		mKeyboardInputConverter.emplace(0x25, KeyboardInputType::SEKey_K);
		mKeyboardInputConverter.emplace(0x26, KeyboardInputType::SEKey_L);
		mKeyboardInputConverter.emplace(0x2C, KeyboardInputType::SEKey_Z);
		mKeyboardInputConverter.emplace(0x2D, KeyboardInputType::SEKey_X);
		mKeyboardInputConverter.emplace(0x2E, KeyboardInputType::SEKey_C);
		mKeyboardInputConverter.emplace(0x2F, KeyboardInputType::SEKey_V);
		mKeyboardInputConverter.emplace(0x30, KeyboardInputType::SEKey_B);
		mKeyboardInputConverter.emplace(0x31, KeyboardInputType::SEKey_N);
		mKeyboardInputConverter.emplace(0x32, KeyboardInputType::SEKey_M);


		mMouseInputConverter.emplace(VK_LBUTTON, MouseInputType::SEMouseLeft);
		mMouseInputConverter.emplace(VK_RBUTTON, MouseInputType::SEMouseRight);
		mMouseInputConverter.emplace(VK_MBUTTON, MouseInputType::SEMouseMiddle);
		mMouseInputConverter.emplace(VK_XBUTTON1, MouseInputType::SEMouseButton4);
		mMouseInputConverter.emplace(VK_XBUTTON2, MouseInputType::SEMouseButton5);

		mGamepadInputConverter.emplace(VK_GAMEPAD_A, GamepadInputType::SEGamepad_FaceButtonDown);
		mGamepadInputConverter.emplace(VK_GAMEPAD_B, GamepadInputType::SEGamepad_FaceButtonRight);
		mGamepadInputConverter.emplace(VK_GAMEPAD_X, GamepadInputType::SEGamepad_FaceButtonLeft);
		mGamepadInputConverter.emplace(VK_GAMEPAD_Y, GamepadInputType::SEGamepad_FaceButtonUp);
		mGamepadInputConverter.emplace(VK_GAMEPAD_DPAD_UP, GamepadInputType::SEGamepad_DPadUp);
		mGamepadInputConverter.emplace(VK_GAMEPAD_DPAD_DOWN, GamepadInputType::SEGamepad_DPadDown);
		mGamepadInputConverter.emplace(VK_GAMEPAD_DPAD_LEFT, GamepadInputType::SEGamepad_DPadLeft);
		mGamepadInputConverter.emplace(VK_GAMEPAD_DPAD_RIGHT, GamepadInputType::SEGamepad_DPadRight);
		mGamepadInputConverter.emplace(VK_GAMEPAD_MENU, GamepadInputType::SEGamepad_Start);
		mGamepadInputConverter.emplace(VK_GAMEPAD_VIEW, GamepadInputType::SEGamepad_Select);
		mGamepadInputConverter.emplace(VK_GAMEPAD_RIGHT_TRIGGER, GamepadInputType::SEGamepad_RightTrigger);
		mGamepadInputConverter.emplace(VK_GAMEPAD_LEFT_TRIGGER, GamepadInputType::SEGamepad_LeftTrigger);
		mGamepadInputConverter.emplace(VK_GAMEPAD_LEFT_SHOULDER, GamepadInputType::SEGamepad_RightBumper);
		mGamepadInputConverter.emplace(VK_GAMEPAD_RIGHT_SHOULDER, GamepadInputType::SEGamepad_RightBumper);
		mGamepadInputConverter.emplace(VK_GAMEPAD_LEFT_THUMBSTICK_BUTTON, GamepadInputType::SEGamepad_LeftStickDown);
		mGamepadInputConverter.emplace(VK_GAMEPAD_RIGHT_THUMBSTICK_BUTTON, GamepadInputType::SEGamepad_RightStickDown);
	}

	void InputManagerWindows::setLockedMousePlatformImpl(bool locked)
	{
		mIsMouseLockedToWindow = locked;
	}

	void InputManagerWindows::setShowSystemCursorPlatformImpl(bool showCursor)
	{
		mShowMouseCursor = showCursor;
		ShowCursor(mShowMouseCursor);
	}

	void InputManagerWindows::setMouseClipToWindowPlatformImpl(bool clip)
	{
		mShouldClipMouseToWindow = clip;
	}
	void InputManagerWindows::setRawInputPlatformImpl(bool useRawInput)
	{
		bool lastState = mUsingRawInput;
		mUsingRawInput = useRawInput;
		if (mUsingRawInput && !(lastState))
		{
			CreateRawInputKeyBoardAndMouse();
		}
		else if (!mUsingRawInput && (lastState))
		{
			DestroyRawInputKeyboardAndMouse();
		}
	}

	bool InputManagerWindows::onInitalize()
	{
		// Check if we have already initialized the system
		if (mInitialized)
		{
			LOG("[Input::Input Manager Windows] System already initialized.");
			return false;
		}
		LOG("[Core::Input Manager Windows] Initializing...");
		Application* app = Application::get();
		HWND handle = (HWND) app->getWindow()->getNativeWindow();
		if (mUsingRawInput)
		{
			CreateRawInputKeyBoardAndMouse();			
			ClipToWindow(handle);
			setMouseClipToWindow(true);
			showSystemCursor(false);
		}
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
			hr = mGamePadDevice->SetCooperativeLevel(handle, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
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
		return true;
	}

	bool InputManagerWindows::onTeardown()
	{
		if (mUsingRawInput)
		{
			DestroyRawInputKeyboardAndMouse();
		}
		// Release devices
		if (mGamePadDevice != nullptr)
		{
			mGamePadDevice->Unacquire();
			mGamePadDevice->Release();
			mGamePadDevice = nullptr;
		}
		SafeRelease(mDirectInput);
		return true;
	}

	void InputManagerWindows::registerEventBindings()
	{
		Application* app = Application::get();
		app->registerEventFunc(Core::EventType::KeyPressed, BIND_EVENT_FN(InputManagerWindows::handleKeyPressedEvent));
		app->registerEventFunc(Core::EventType::KeyReleased, BIND_EVENT_FN(InputManagerWindows::handleKeyReleasedEvent));
		app->registerEventFunc(Core::EventType::MouseButtonPressed, BIND_EVENT_FN(InputManagerWindows::handleMouseButtonPressedEvent));
		app->registerEventFunc(Core::EventType::MouseButtonReleased, BIND_EVENT_FN(InputManagerWindows::handleMouseButtonReleasedEvent));
		app->registerEventFunc(Core::EventType::MouseMoved, BIND_EVENT_FN(InputManagerWindows::handleMouseMovedEvent));
		app->registerEventFunc(Core::EventType::MouseScrolled, BIND_EVENT_FN(InputManagerWindows::handleMouseScrolledEvent));
		app->registerEventFunc(Core::EventType::GamepadButtonPressed, BIND_EVENT_FN(InputManagerWindows::handleGamepadButtonPressedEvent));
		app->registerEventFunc(Core::EventType::GamepadButtonReleased, BIND_EVENT_FN(InputManagerWindows::handleGamepadButtonReleasedEvent));
		app->registerEventFunc(Core::EventType::GamepadJoystickMoved, BIND_EVENT_FN(InputManagerWindows::handleGamepadJoystickMovedEvent));

	}

	bool InputManagerWindows::handleKeyPressedEvent(Event& e)
	{
		auto& keyEvent = dynamic_cast<KeyPressedEvent&>(e);
		const auto& iter = mKeyboardInputConverter.find(keyEvent.GetKeyCode());
		if (iter != mKeyboardInputConverter.end())
		{
			KeyboardInputType input = iter->second;
			mCurrentKeyStateBuffer[(size_t)input] = true;
			return true;
		}

		return false;
	}

	bool InputManagerWindows::handleKeyReleasedEvent(Event& e)
	{
		auto& keyEvent = dynamic_cast<KeyReleasedEvent&>(e);
		const auto& iter = mKeyboardInputConverter.find(keyEvent.GetKeyCode());
		if (iter != mKeyboardInputConverter.end())
		{
			KeyboardInputType input = iter->second;
			mCurrentKeyStateBuffer[(size_t)input] = false;
			return true;
		}

		return false;
	}

	bool InputManagerWindows::handleMouseButtonPressedEvent(Event& e)
	{
		auto& mouseEvent = dynamic_cast<MousePressedEvent&>(e);
		const auto& iter = mMouseInputConverter.find(mouseEvent.getMouseButton());
		if (iter != mMouseInputConverter.end())
		{
			MouseInputType input = iter->second;
			mCurrentMouseButtons[(size_t)input] = true;
			return true;
		}

		return false;
	}

	bool InputManagerWindows::handleMouseButtonReleasedEvent(Event& e)
	{
		auto& mouseEvent = dynamic_cast<MouseReleasedEvent&>(e);
		const auto& iter = mMouseInputConverter.find(mouseEvent.getMouseButton());
		if (iter != mMouseInputConverter.end())
		{
			MouseInputType input = iter->second;
			mCurrentMouseButtons[(size_t)input] = false;
			return true;
		}

		return false;
	}

	bool InputManagerWindows::handleMouseMovedEvent(Event& e)
	{
		auto& mouseEvent = dynamic_cast<MouseMovedEvent&>(e);
		if (mIsMouseLockedToWindow)
		{
			mMouseDeltaX = mouseEvent.getX();
			mMouseDeltaY = mouseEvent.getY();
			return true;
		}
		mMouseScreenPositionX = mouseEvent.getX();
		mMouseScreenPositionY = mouseEvent.getY();
		return true;
	}

	bool InputManagerWindows::handleMouseScrolledEvent(Event& e)
	{
		auto& mouseEvent = dynamic_cast<MouseScrolledEvent&>(e);
		if (mUsingRawInput)
		{
			mMouseDeltaX = mouseEvent.getX();
			mMouseDeltaY = mouseEvent.getY();
			return true;
		}
		mMouseWheelX = mouseEvent.getX();
		mMouseWheelY = mouseEvent.getY();
		return true;
	}

	bool InputManagerWindows::handleGamepadButtonPressedEvent(Event& e)
	{
		auto& gamepadEvent = dynamic_cast<GamepadButtonPressedEvent&>(e);
		const auto& iter = mGamepadInputConverter.find(gamepadEvent.getButton());
		if (iter != mGamepadInputConverter.end())
		{
			GamepadInputType input = iter->second;
			mPressedGamepadButtons[(size_t)input] = true;
			return true;
		}

		return false;
	}

	bool InputManagerWindows::handleGamepadButtonReleasedEvent(Event& e)
	{
		auto& gamepadEvent = dynamic_cast<GamepadButtonReleasedEvent&>(e);
		const auto& iter = mGamepadInputConverter.find(gamepadEvent.getButton());
		if (iter != mGamepadInputConverter.end())
		{
			GamepadInputType input = iter->second;
			mPressedGamepadButtons[(size_t)input] = false;
			return true;
		}

		return false;
	}

	bool InputManagerWindows::handleGamepadJoystickMovedEvent(Event& e)
	{
		auto& gamepadEvent = dynamic_cast<GamepadJoystickMovedEvent&>(e);
		if (gamepadEvent.isLeftStick())
		{
			//TODO: add support for multiple gamepads here, use index from event to find gamepads info to change
			mLeftJoystickPosX = gamepadEvent.getX();
			mLeftJoystickPosY = gamepadEvent.getY();
		}
		else
		{
			mRightJoystickPosX = gamepadEvent.getX();
			mRightJoystickPosY = gamepadEvent.getY();
		}
		return true;
	}

	void InputManagerWindows::ClipToWindow(HWND handle)
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

	void InputManagerWindows::onPollInput()
	{
		ASSERT(mInitialized, "[Core::Input] System not initialized.");
		// Update game pad
		if (mGamePadDevice != nullptr)
		{
			pollGamepadInput();
		}
		// Store the previous keyboard state
		size_t sizeOfKeyBuffer = (size_t)KeyboardInputType::KEYBOARD_INPUT_MAX;
		for (size_t i = 0; i < sizeOfKeyBuffer; ++i)
		{
			mPressedKeys[i] = !mLastKeyStateBuffer[i] && mCurrentKeyStateBuffer[i];
		}
		if (!mIsMouseLockedToWindow)
		{
			// Update mouse movement
			mMouseDeltaX = mMouseScreenPositionX - mLastMouseScreenPositionX;
			mMouseDeltaY = mMouseScreenPositionY - mLastMouseScreenPositionY;
			mLastMouseScreenPositionX = mMouseScreenPositionX;
			mLastMouseScreenPositionY = mMouseScreenPositionY;
		}
		mDeltaMouseWheelY = mMouseWheelY - mLastMouseWheelY;
		mLastMouseWheelY = mMouseWheelY;
		mDeltaMouseWheelX = mMouseWheelX - mLastMouseWheelX;
		mLastMouseWheelX = mMouseWheelX;
		// Store the previous mouse state
		for (int i = 0; i < 3; ++i)
		{
			mPressedMouseButtons[i] = !mLastMouseButtons[i] && mCurrentMouseButtons[i];
		}
	}

	void InputManagerWindows::pollGamepadInput()
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

		//TODO: Handle shipping out events for state changes on gamepad
	}

}