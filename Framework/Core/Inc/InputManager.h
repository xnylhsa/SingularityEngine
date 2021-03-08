#ifndef SINGULARITY_ENGINE_INPUT_MANAGER
#define SINGULARITY_ENGINE_INPUT_MANAGER

#include "Common.h"
#include "SEInputTypes.h"
#include "Event.h"
namespace SingularityEngine::Core
{
	class InputManager
	{
	public:
		using GamepadConverter = std::unordered_map<int, GamepadInputType>;
		using MouseConverter = std::unordered_map<int, MouseInputType>;
		using KeyboardConverter = std::unordered_map<int, KeyboardInputType>;

		static bool Initalize();
		static bool Teardown();
		static InputManager* get();
		static void pollInput();
		inline static bool isInitalized() { return sInstance->mInitialized; }
		inline static bool isKeyPressed(KeyboardInputType input) { return sInstance->isKeyPressedPlatformImpl(input); }
		inline static bool isKeyHeld(KeyboardInputType input) { return sInstance->isKeyHeldPlatformImpl(input); }
		inline static bool isMouseButtonPressed(MouseInputType input) { return sInstance->isMouseButtonPressedPlatformImpl(input); }
		inline std::pair<float, float> getMousePos() { return { getMouseX(), getMouseY() }; }
		inline static float getMouseX() { return sInstance->getMouseXPlatformImpl(); }
		inline static float getMouseY() { return sInstance->getMouseYPlatformImpl(); }
		inline std::pair<float, float> getMouseDelta() { return { getMouseX(), getMouseY() }; }
		inline static float getMouseDeltaX() { return sInstance->getMouseDeltaXPlatformImpl(); }
		inline static float getMouseDeltaY() { return sInstance->getMouseDeltaYPlatformImpl(); }
		inline static bool isMouseTopEdge() { return sInstance->isMouseTopEdgePlatformImpl(); }
		inline static bool isMouseLeftEdge() { return sInstance->isMouseLeftEdgePlatformImpl(); }
		inline static bool isMouseRightEdge() { return sInstance->isMouseRightEdgePlatformImpl(); }
		inline static bool isMouseBottomEdge() { return sInstance->isMouseBottomEdgePlatformImpl(); }
		inline static bool isGamePadButtonDown(GamepadInputType button) { return sInstance->isGamepadButtonDownPlatformImpl(button); }
		inline static bool isGamePadButtonPressed(GamepadInputType button) { return sInstance->isGamepadButtonPressedPlatformImpl(button); }
		inline static void setLockedMouse(bool lock) { sInstance->setLockedMousePlatformImpl(lock); }
		inline static void showSystemCursor(bool show) { sInstance->setShowSystemCursorPlatformImpl(show); }
		inline static void setMouseClipToWindow(bool clip) { sInstance->setMouseClipToWindowPlatformImpl(clip); }
		inline static void setUseRawInput(bool useRawInput) { sInstance->setRawInputPlatformImpl(useRawInput); }
		inline static bool isUsingRawInput() { return sInstance->mUsingRawInput; }
		inline static bool isMouseLockedToWindow() { return sInstance->mIsMouseLockedToWindow; }
		inline static bool isClippingMouseToWindow() { return sInstance->mShouldClipMouseToWindow; }
		inline static bool isShowingMouseCursor() { return sInstance->mShowMouseCursor; }
	protected:
		virtual void registerKeyMappings() = 0;
		virtual void registerEventBindings() = 0;
		virtual bool isKeyPressedPlatformImpl(KeyboardInputType input) const;
		virtual bool isKeyHeldPlatformImpl(KeyboardInputType input) const;

		virtual bool isMouseButtonPressedPlatformImpl(MouseInputType input) const;
		virtual float getMouseXPlatformImpl() const;
		virtual float getMouseYPlatformImpl() const;
		virtual float getMouseDeltaXPlatformImpl() const;
		virtual float getMouseDeltaYPlatformImpl() const;
		virtual bool isMouseLeftEdgePlatformImpl() const;
		virtual bool isMouseRightEdgePlatformImpl() const;
		virtual bool isMouseTopEdgePlatformImpl() const;
		virtual bool isMouseBottomEdgePlatformImpl() const;
		virtual bool isGamepadButtonPressedPlatformImpl(GamepadInputType input);
		virtual bool isGamepadButtonDownPlatformImpl(GamepadInputType input);
		virtual void setLockedMousePlatformImpl(bool locked) = 0;
		virtual void setShowSystemCursorPlatformImpl(bool showCursor) = 0;
		virtual void setMouseClipToWindowPlatformImpl(bool clip) = 0;
		virtual void setRawInputPlatformImpl(bool useRawInput) = 0;
		virtual bool onInitalize() = 0;
		virtual bool onTeardown() = 0;
		virtual bool handleKeyPressedEvent(Event& e) = 0;
		virtual bool handleKeyReleasedEvent(Event& e) = 0;
		virtual bool handleMouseButtonPressedEvent(Event& e) = 0;
		virtual bool handleMouseButtonReleasedEvent(Event& e) = 0;
		virtual bool handleMouseMovedEvent(Event& e) = 0;
		virtual bool handleMouseScrolledEvent(Event& e) = 0;
		virtual bool handleGamepadButtonPressedEvent(Event& e) = 0;
		virtual bool handleGamepadButtonReleasedEvent(Event& e) = 0;
		virtual bool handleGamepadJoystickMovedEvent(Event& e) = 0;
		virtual void onPollInput() = 0;


		//Native Windows Key Buffers, first one stores the current state of all keys Now, the other stores them previous frame, you can then find out if the key was just tapped, if it was held
		bool mCurrentKeyStateBuffer[(size_t)KeyboardInputType::KEYBOARD_INPUT_MAX] = {0};
		bool mLastKeyStateBuffer[(size_t)KeyboardInputType::KEYBOARD_INPUT_MAX] = {0};
		bool mPressedKeys[(size_t)KeyboardInputType::KEYBOARD_INPUT_MAX] = {0};

		//Round 2 Electric Boogaloo, Mouse Edition
		bool mCurrentMouseButtons[(size_t)MouseInputType::MOUSE_INPUT_MAX] = {0};
		bool mLastMouseButtons[(size_t)MouseInputType::MOUSE_INPUT_MAX] = { 0 };
		bool mPressedMouseButtons[(size_t)MouseInputType::MOUSE_INPUT_MAX] = { 0 };
		float mMouseScreenPositionX;
		float mMouseScreenPositionY;
		float mLastMouseScreenPositionX;
		float mLastMouseScreenPositionY;
		float mMouseDeltaX;
		float mMouseDeltaY;
		float mMouseWheelX{ -1 };
		float mLastMouseWheelX{ -1 };
		float mDeltaMouseWheelX{ 0 };
		float mMouseWheelY{ -1 };
		float mLastMouseWheelY{ -1 };
		float mDeltaMouseWheelY{ 0 };
		bool mMouseLeftEdge;
		bool mMouseRightEdge;
		bool mMouseTopEdge;
		bool mMouseBottomEdge;
		bool mIsMouseLockedToWindow = false;
		bool mShowMouseCursor = false;
		bool mShouldClipMouseToWindow = false;


		bool mCurrentGamepadButtons[(size_t)GamepadInputType::GAMEPAD_INPUT_MAX] = { 0 };
		bool mLastGamepadButtons[(size_t)GamepadInputType::GAMEPAD_INPUT_MAX] = { 0 };
		bool mPressedGamepadButtons[(size_t)GamepadInputType::GAMEPAD_INPUT_MAX] = { 0 };
		float mLastLeftJoystickPosX;
		float mLeftJoystickPosX;
		float mDeltaLeftJoystickX;
		float mLastLeftJoystickPosY;
		float mLeftJoystickPosY;
		float mDeltaLeftJoystickY;
		float mLastRightJoystickPosX;
		float mRightJoystickPosX;
		float mDeltaRightJoystickX;
		float mLastRightJoystickPosY;
		float mRightJoystickPosY;
		float mDeltaRightJoystickY;

		bool mUsingRawInput = false;
		bool mInitialized = false;
		GamepadConverter mGamepadInputConverter;
		MouseConverter mMouseInputConverter;
		KeyboardConverter mKeyboardInputConverter;
	private:

		static InputManager* sInstance;
	};
}


#endif