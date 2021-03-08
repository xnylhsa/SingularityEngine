#ifndef SINGULARITY_ENGINE_INPUT_MANAGER_WINDOWS
#define SINGULARITY_ENGINE_INPUT_MANAGER_WINDOWS
#include "InputManager.h"

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

#ifndef HID_USAGE_PAGE_GENERIC
#define HID_USAGE_PAGE_GENERIC         ((USHORT) 0x01)
#endif
#ifndef HID_USAGE_GENERIC_MOUSE
#define HID_USAGE_GENERIC_MOUSE        ((USHORT) 0x02)
#endif

#ifndef HID_USAGE_GENERIC_KEYBOARD
#define HID_USAGE_GENERIC_KEYBOARD        ((USHORT) 0x06)
#endif
namespace SingularityEngine::Core
{
	class Application;
	class InputManagerWindows : public InputManager
	{
	public:
		InputManagerWindows() = default;
		~InputManagerWindows() = default;
	protected:
		static BOOL CALLBACK EnumGamePadCallback(const DIDEVICEINSTANCE* pDIDeviceInstance, VOID* pContext);

		void registerKeyMappings() override;


		void setLockedMousePlatformImpl(bool locked) override;


		void setShowSystemCursorPlatformImpl(bool showCursor) override;


		void setMouseClipToWindowPlatformImpl(bool clip) override;

		void setRawInputPlatformImpl(bool useRawInput) override;

		bool onInitalize() override;


		bool onTeardown() override;


		void registerEventBindings() override;


		bool handleKeyPressedEvent(Event& e) override;


		bool handleKeyReleasedEvent(Event& e) override;


		bool handleMouseButtonPressedEvent(Event& e) override;


		bool handleMouseButtonReleasedEvent(Event& e) override;


		bool handleMouseMovedEvent(Event& e) override;


		bool handleMouseScrolledEvent(Event& e) override;


		bool handleGamepadButtonPressedEvent(Event& e) override;


		bool handleGamepadButtonReleasedEvent(Event& e) override;


		bool handleGamepadJoystickMovedEvent(Event& e) override;


		void onPollInput() override;

	private:
		void ClipToWindow(HWND handle);
		void CreateRawInputKeyBoardAndMouse();
		void DestroyRawInputKeyboardAndMouse();
		void pollGamepadInput();
		RAWINPUTDEVICE mRawInputDevices[2];

		//Direct Input Devices for Xbox/Gamecube and other types of joysticks
		IDirectInput8* mDirectInput = nullptr;
		IDirectInputDevice8* mGamePadDevice = nullptr; //make some sort of handle system for joysticks and abstract them so that i can use them on any platform
		DIJOYSTATE mCurrGamePadState;
		DIJOYSTATE mPrevGamePadState;
	};
}

#endif