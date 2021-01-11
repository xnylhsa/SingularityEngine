#ifndef INCLUDED_INPUT_INPUTSYSTEM_H
#define INCLUDED_INPUT_INPUTSYSTEM_H

#ifndef HID_USAGE_PAGE_GENERIC
#define HID_USAGE_PAGE_GENERIC         ((USHORT) 0x01)
#endif
#ifndef HID_USAGE_GENERIC_MOUSE
#define HID_USAGE_GENERIC_MOUSE        ((USHORT) 0x02)
#endif

#ifndef HID_USAGE_GENERIC_KEYBOARD
#define HID_USAGE_GENERIC_KEYBOARD        ((USHORT) 0x06)
#endif



#include "InputTypes.h"
namespace SingularityEngine
{

namespace Input {

class InputSystem
{
public:
	static void StaticInitialize(HWND window, bool useRawInput = false);
	static void StaticTerminate();
	static InputSystem* Get();
	static LRESULT ProcessMessage(HWND window, UINT msg, WPARAM wParam, LPARAM lParam);
public:
	InputSystem();
	~InputSystem();

	void Initialize(HWND window, bool useRawInput);
	void Terminate();

	void Update();

	bool IsKeyDown(uint32_t key) const;
	bool IsKeyPressed(uint32_t key) const;

	bool IsMouseDown(uint32_t button) const;
	bool IsMousePressed(uint32_t button) const;

	int GetMouseScreenX() const;
	int GetMouseScreenY() const;

	bool IsMouseLeftEdge() const;
	bool IsMouseRightEdge() const;
	bool IsMouseTopEdge() const;
	bool IsMouseBottomEdge() const;

	int GetMouseMoveX() const;
	int GetMouseMoveY() const;
	float GetMouseScrollWheel() const;

	bool IsGamePadButtonDown(GamePadButton button) const;
	bool IsGamePadButtonPressed(GamePadButton button) const;
	void SetLockedMouse(bool lock);
	void ShowSystemCursor(bool show);
	void SetMouseClipToWindow(bool clip);
	bool IsDPadUp() const;
	bool IsDPadDown() const;
	bool IsDPadLeft() const;
	bool IsDPadRight() const;

	bool IsMouseClippedToWindow() const;
	bool UsingRawInput() const;

	float GetLeftAnalogX() const;
	float GetLeftAnalogY() const;
	float GetRightAnalogX() const;
	float GetRightAnalogY() const;

private:
	InputSystem(const InputSystem&) = delete;
	InputSystem& operator=(const InputSystem&) = delete;

	friend BOOL CALLBACK EnumGamePadCallback(const DIDEVICEINSTANCE* pDIDeviceInstance, VOID* pContext);

	void UpdateKeyboard();
	void UpdateMouse();
	void UpdateGamePad();


	void CreateRawInputKeyBoardAndMouse();
	void DestroyRawInputKeyboardAndMouse();
	HWND mWindow;
	//Direct Input Devices for Xbox/Gamecube and other types of joysticks
	IDirectInput8* mDirectInput;
	IDirectInputDevice8* mGamePadDevice;
	DIJOYSTATE mCurrGamePadState;
	DIJOYSTATE mPrevGamePadState;

	//Natrive Windows Key Buffers, first one stores the current state of all keys Now, the other stores them previous frame, you can then find out if the key was just tapped, if it was held
	bool mCurrentKeyStateBuffer[512];
	bool mLastKeyStateBuffer[512];
	bool mPressedKeys[512];

	//Round 2 Electric Boogaloo, Mouse Edition
	bool mCurrentMouseButtons[5];
	bool mLastMouseButtons[5];
	bool mPressedMouseButtons[5];

	int mMouseScreenPositionX;
	int mMouseScreenPositionY;
	int mLastMouseScreenPositionX;
	int mLastMouseScreenPositionY;

	int mMouseDeltaX;
	int mMouseDeltaY;

	RAWINPUTDEVICE mRawInputDevices[2];

	float mMouseWheel{ -1 };
	float mLastMouseWheel{ -1 };
	float mDeltaMouseWheel{ 0 };

	bool mMouseLeftEdge;
	bool mMouseRightEdge;
	bool mMouseTopEdge;
	bool mMouseBottomEdge;

	bool mClipMouseToWindow;
	bool mLockedMouse = false;
	bool mUsingRawInput = false;
	bool mInitialized;

};

} // namespace Input
} // namespace Singularity Engine

#endif // #ifndef INCLUDED_INPUT_INPUTSYSTEM_H