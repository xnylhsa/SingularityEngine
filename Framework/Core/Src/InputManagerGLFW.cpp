#include "precompiled.h"
#include "InputManagerGLFW.h"
#include "GamepadEvent.h"
#include "KeyEvent.h"
#include "MouseEvent.h"
#include "Application.h"
#include "GLFWWindow.h"
namespace SingularityEngine::Core
{
#ifdef SINGULARITY_USE_GLFW
	InputManager* InputManager::sInstance = new InputManagerGLFW();
#endif

	bool InputManagerGLFW::handleGamepadJoystickMovedEvent(Event& e)
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

	bool InputManagerGLFW::handleGamepadButtonReleasedEvent(Event& e)
	{
		auto& gamepadEvent = dynamic_cast<GamepadButtonReleasedEvent&>(e);
		const auto& iter = mGamepadInputConverter.find(gamepadEvent.getButton());
		if (iter != mGamepadInputConverter.end())
		{
			GamepadInputType input = iter->second;
			mCurrentGamepadButtons[(size_t)input] = false;
			return true;
		}

		return false;
	}

	bool InputManagerGLFW::handleGamepadButtonPressedEvent(Event& e)
	{
		auto& gamepadEvent = dynamic_cast<GamepadButtonPressedEvent&>(e);
		const auto& iter = mGamepadInputConverter.find(gamepadEvent.getButton());
		if (iter != mGamepadInputConverter.end())
		{
			GamepadInputType input = iter->second;
			mCurrentGamepadButtons[(size_t)input] = true;
			return true;
		}

		return false;
	}

	bool InputManagerGLFW::handleMouseScrolledEvent(Event& e)
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

	bool InputManagerGLFW::handleMouseMovedEvent(Event& e)
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
		Window* window = Application::get()->getWindow();

		mMouseLeftEdge = mMouseScreenPositionX <= 0;
		mMouseRightEdge = mMouseScreenPositionX + 1 >= window->getWidth();
		mMouseTopEdge = mMouseScreenPositionY <= 0;
		mMouseBottomEdge = mMouseScreenPositionY + 1 >= window->getHeight();
		if (mShouldClipMouseToWindow)
		{
			if (mMouseLeftEdge)
			{
				glfwSetCursorPos((GLFWwindow*)window->getNativeWindow(), 0, mMouseScreenPositionY);
			}
			else if (mMouseRightEdge)
			{
				glfwSetCursorPos((GLFWwindow*)window->getNativeWindow(), window->getWidth(), mMouseScreenPositionY);
			}
			if (mMouseTopEdge)
			{
				glfwSetCursorPos((GLFWwindow*)window->getNativeWindow(), mMouseScreenPositionX, 0);
			}
			else if (mMouseBottomEdge)
			{
				glfwSetCursorPos((GLFWwindow*)window->getNativeWindow(), mMouseScreenPositionX, window->getHeight());
			}
		}
		return true;
	}

	bool InputManagerGLFW::handleMouseButtonReleasedEvent(Event& e)
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

	bool InputManagerGLFW::handleMouseButtonPressedEvent(Event& e)
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

	bool InputManagerGLFW::handleKeyReleasedEvent(Event& e)
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

	bool InputManagerGLFW::handleKeyPressedEvent(Event& e)
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

	bool InputManagerGLFW::onTeardown()
	{
		return true;
	}

	bool InputManagerGLFW::onInitalize()
	{
		return true;
	}

	void InputManagerGLFW::setRawInputPlatformImpl(bool useRawInput)
	{
		bool lastState = mUsingRawInput;
		mUsingRawInput = useRawInput;
		if (mUsingRawInput && !lastState && glfwRawMouseMotionSupported())
		{
			Application* app = Application::get();
			Window* myWindow = app->getWindow();
			auto* window = (GLFWwindow*)(myWindow->getNativeWindow());
			glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
		}
		else
		{
			Application* app = Application::get();
			Window* myWindow = app->getWindow();

			auto* window = (GLFWwindow*)(myWindow->getNativeWindow());
			glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_FALSE);
		}
	}

	void InputManagerGLFW::setMouseClipToWindowPlatformImpl(bool clip)
	{
		mShouldClipMouseToWindow = clip;
		Application* app = Application::get();
		Window* myWindow = app->getWindow();
		auto* window = (GLFWwindow*)(myWindow->getNativeWindow());

		if (mShouldClipMouseToWindow)
		{
			mCursor = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
			glfwSetCursor(window, mCursor);
		}
		else
		{
			glfwSetCursor(window, nullptr);
			glfwDestroyCursor(mCursor);
		}
	}

	void InputManagerGLFW::setShowSystemCursorPlatformImpl(bool showCursor)
	{
		mShowMouseCursor = showCursor;
		if (mShowMouseCursor && !mIsMouseLockedToWindow)
		{
			Application* app = Application::get();
			Window* myWindow = app->getWindow();
			auto* window = (GLFWwindow*)(myWindow->getNativeWindow());

			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}
		else if(!mIsMouseLockedToWindow)
		{
			Application* app = Application::get();
			Window* myWindow = app->getWindow();
			auto* window = (GLFWwindow*)(myWindow->getNativeWindow());
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
		}
	}

	void InputManagerGLFW::setLockedMousePlatformImpl(bool locked)
	{
		mIsMouseLockedToWindow = locked;
		if (mIsMouseLockedToWindow)
		{
			Application* app = Application::get();
			Window* myWindow = app->getWindow();
			auto* window = (GLFWwindow*)(myWindow->getNativeWindow());

			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}
		else
		{
			Application* app = Application::get();
			Window* myWindow = app->getWindow();
			auto* window = (GLFWwindow*)(myWindow->getNativeWindow());		
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}
	}

	void InputManagerGLFW::registerEventBindings()
	{
		Application* app = Application::get();
		app->registerEventFunc(Core::EventType::KeyPressed, BIND_EVENT_FN(InputManagerGLFW::handleKeyPressedEvent));
		app->registerEventFunc(Core::EventType::KeyReleased, BIND_EVENT_FN(InputManagerGLFW::handleKeyReleasedEvent));
		app->registerEventFunc(Core::EventType::MouseButtonPressed, BIND_EVENT_FN(InputManagerGLFW::handleMouseButtonPressedEvent));
		app->registerEventFunc(Core::EventType::MouseButtonReleased, BIND_EVENT_FN(InputManagerGLFW::handleMouseButtonReleasedEvent));
		app->registerEventFunc(Core::EventType::MouseMoved, BIND_EVENT_FN(InputManagerGLFW::handleMouseMovedEvent));
		app->registerEventFunc(Core::EventType::MouseScrolled, BIND_EVENT_FN(InputManagerGLFW::handleMouseScrolledEvent));
		app->registerEventFunc(Core::EventType::GamepadButtonPressed, BIND_EVENT_FN(InputManagerGLFW::handleGamepadButtonPressedEvent));
		app->registerEventFunc(Core::EventType::GamepadButtonReleased, BIND_EVENT_FN(InputManagerGLFW::handleGamepadButtonReleasedEvent));
		app->registerEventFunc(Core::EventType::GamepadJoystickMoved, BIND_EVENT_FN(InputManagerGLFW::handleGamepadJoystickMovedEvent));
	}

	void InputManagerGLFW::registerKeyMappings()
	{
		mKeyboardInputConverter.emplace(GLFW_KEY_ESCAPE, KeyboardInputType::SEKey_Escape);
		mKeyboardInputConverter.emplace(GLFW_KEY_F1, KeyboardInputType::SEKey_F1);
		mKeyboardInputConverter.emplace(GLFW_KEY_F2, KeyboardInputType::SEKey_F2);
		mKeyboardInputConverter.emplace(GLFW_KEY_F3, KeyboardInputType::SEKey_F3);
		mKeyboardInputConverter.emplace(GLFW_KEY_F4, KeyboardInputType::SEKey_F4);
		mKeyboardInputConverter.emplace(GLFW_KEY_F5, KeyboardInputType::SEKey_F5);
		mKeyboardInputConverter.emplace(GLFW_KEY_F6, KeyboardInputType::SEKey_F6);
		mKeyboardInputConverter.emplace(GLFW_KEY_F7, KeyboardInputType::SEKey_F7);
		mKeyboardInputConverter.emplace(GLFW_KEY_F8, KeyboardInputType::SEKey_F8);
		mKeyboardInputConverter.emplace(GLFW_KEY_F9, KeyboardInputType::SEKey_F9);
		mKeyboardInputConverter.emplace(GLFW_KEY_F10, KeyboardInputType::SEKey_F10);
		mKeyboardInputConverter.emplace(GLFW_KEY_F11, KeyboardInputType::SEKey_F11);
		mKeyboardInputConverter.emplace(GLFW_KEY_F12, KeyboardInputType::SEKey_F12);

		mKeyboardInputConverter.emplace(GLFW_KEY_GRAVE_ACCENT, KeyboardInputType::SEKey_Grave);
		mKeyboardInputConverter.emplace(GLFW_KEY_MINUS, KeyboardInputType::SEKey_Minus);
		mKeyboardInputConverter.emplace(GLFW_KEY_EQUAL, KeyboardInputType::SEKey_Equals);
		mKeyboardInputConverter.emplace(GLFW_KEY_BACKSPACE, KeyboardInputType::SEKey_Backspace);
		mKeyboardInputConverter.emplace(GLFW_KEY_LEFT_BRACKET, KeyboardInputType::SEKey_LeftBracket);
		mKeyboardInputConverter.emplace(GLFW_KEY_RIGHT_BRACKET, KeyboardInputType::SEKey_RightBracket);
		mKeyboardInputConverter.emplace(GLFW_KEY_SEMICOLON, KeyboardInputType::SEKey_SemiColon);
		mKeyboardInputConverter.emplace(GLFW_KEY_APOSTROPHE, KeyboardInputType::SEKey_Apostrophe);
		mKeyboardInputConverter.emplace(GLFW_KEY_ENTER, KeyboardInputType::SEKey_Enter);
		mKeyboardInputConverter.emplace(GLFW_KEY_COMMA, KeyboardInputType::SEKey_Comma);
		mKeyboardInputConverter.emplace(GLFW_KEY_PERIOD, KeyboardInputType::SEKey_Period);
		mKeyboardInputConverter.emplace(GLFW_KEY_SLASH, KeyboardInputType::SEKey_ForwardSlash);
		mKeyboardInputConverter.emplace(GLFW_KEY_CAPS_LOCK, KeyboardInputType::SEKey_CapsLock);
		mKeyboardInputConverter.emplace(GLFW_KEY_NUM_LOCK, KeyboardInputType::SEKey_NumLock);
		mKeyboardInputConverter.emplace(GLFW_KEY_SCROLL_LOCK, KeyboardInputType::SEKey_ScrollLock);
		mKeyboardInputConverter.emplace(GLFW_KEY_INSERT, KeyboardInputType::SEKey_Insert);
		mKeyboardInputConverter.emplace(GLFW_KEY_DELETE, KeyboardInputType::SEKey_Del);
		mKeyboardInputConverter.emplace(GLFW_KEY_HOME, KeyboardInputType::SEKey_Home);
		mKeyboardInputConverter.emplace(GLFW_KEY_END, KeyboardInputType::SEKey_End);
		mKeyboardInputConverter.emplace(GLFW_KEY_PAGE_UP, KeyboardInputType::SEKey_PageUp);
		mKeyboardInputConverter.emplace(GLFW_KEY_PAGE_DOWN, KeyboardInputType::SEKey_PageDown);

		mKeyboardInputConverter.emplace(GLFW_KEY_LEFT_SHIFT, KeyboardInputType::SEKey_LeftShift);
		mKeyboardInputConverter.emplace(GLFW_KEY_RIGHT_SHIFT, KeyboardInputType::SEKey_RightShift);
		mKeyboardInputConverter.emplace(GLFW_KEY_LEFT_CONTROL, KeyboardInputType::SEKey_LeftCtrl);
		mKeyboardInputConverter.emplace(GLFW_KEY_RIGHT_CONTROL, KeyboardInputType::SEKey_RightCtrl);
		mKeyboardInputConverter.emplace(GLFW_KEY_LEFT_ALT, KeyboardInputType::SEKey_LeftAlt);
		mKeyboardInputConverter.emplace(GLFW_KEY_RIGHT_ALT, KeyboardInputType::SEKEY_RightAlt);
		mKeyboardInputConverter.emplace(GLFW_KEY_SPACE, KeyboardInputType::SEKey_Space);

		mKeyboardInputConverter.emplace(GLFW_KEY_UP, KeyboardInputType::SEKey_Up);
		mKeyboardInputConverter.emplace(GLFW_KEY_DOWN, KeyboardInputType::SEKey_Down);
		mKeyboardInputConverter.emplace(GLFW_KEY_LEFT, KeyboardInputType::SEKey_Left);
		mKeyboardInputConverter.emplace(GLFW_KEY_RIGHT, KeyboardInputType::SEKey_Right);

		mKeyboardInputConverter.emplace(GLFW_KEY_KP_0, KeyboardInputType::SENumpad_0);
		mKeyboardInputConverter.emplace(GLFW_KEY_KP_1, KeyboardInputType::SENumpad_1);
		mKeyboardInputConverter.emplace(GLFW_KEY_KP_2, KeyboardInputType::SENumpad_2);
		mKeyboardInputConverter.emplace(GLFW_KEY_KP_3, KeyboardInputType::SENumpad_3);
		mKeyboardInputConverter.emplace(GLFW_KEY_KP_4, KeyboardInputType::SENumpad_4);
		mKeyboardInputConverter.emplace(GLFW_KEY_KP_5, KeyboardInputType::SENumpad_5);
		mKeyboardInputConverter.emplace(GLFW_KEY_KP_6, KeyboardInputType::SENumpad_6);
		mKeyboardInputConverter.emplace(GLFW_KEY_KP_7, KeyboardInputType::SENumpad_7);
		mKeyboardInputConverter.emplace(GLFW_KEY_KP_8, KeyboardInputType::SENumpad_8);
		mKeyboardInputConverter.emplace(GLFW_KEY_KP_9, KeyboardInputType::SENumpad_9);
		mKeyboardInputConverter.emplace(GLFW_KEY_KP_ADD, KeyboardInputType::SENumpad_Add);
		mKeyboardInputConverter.emplace(GLFW_KEY_KP_SUBTRACT, KeyboardInputType::SENumpad_Subtract);
		mKeyboardInputConverter.emplace(GLFW_KEY_KP_MULTIPLY, KeyboardInputType::SENumpad_Multiply);
		mKeyboardInputConverter.emplace(GLFW_KEY_KP_DIVIDE, KeyboardInputType::SENumpad_Divide);
		mKeyboardInputConverter.emplace(GLFW_KEY_KP_ENTER, KeyboardInputType::SENumpad_Enter);
		mKeyboardInputConverter.emplace(GLFW_KEY_KP_DECIMAL, KeyboardInputType::SENumpad_Decimal);
		mKeyboardInputConverter.emplace(GLFW_KEY_TAB, KeyboardInputType::SEKey_Tab);
		mKeyboardInputConverter.emplace(GLFW_KEY_BACKSLASH, KeyboardInputType::SEKey_BackSlash);

		mKeyboardInputConverter.emplace(GLFW_KEY_0, KeyboardInputType::SEKey_0);
		mKeyboardInputConverter.emplace(GLFW_KEY_1, KeyboardInputType::SEKey_1);
		mKeyboardInputConverter.emplace(GLFW_KEY_2, KeyboardInputType::SEKey_2);
		mKeyboardInputConverter.emplace(GLFW_KEY_3, KeyboardInputType::SEKey_3);
		mKeyboardInputConverter.emplace(GLFW_KEY_4, KeyboardInputType::SEKey_4);
		mKeyboardInputConverter.emplace(GLFW_KEY_5, KeyboardInputType::SEKey_5);
		mKeyboardInputConverter.emplace(GLFW_KEY_6, KeyboardInputType::SEKey_6);
		mKeyboardInputConverter.emplace(GLFW_KEY_7, KeyboardInputType::SEKey_7);
		mKeyboardInputConverter.emplace(GLFW_KEY_8, KeyboardInputType::SEKey_8);
		mKeyboardInputConverter.emplace(GLFW_KEY_9, KeyboardInputType::SEKey_9);


		mKeyboardInputConverter.emplace(GLFW_KEY_Q, KeyboardInputType::SEKey_Q);
		mKeyboardInputConverter.emplace(GLFW_KEY_W, KeyboardInputType::SEKey_W);
		mKeyboardInputConverter.emplace(GLFW_KEY_E, KeyboardInputType::SEKey_E);
		mKeyboardInputConverter.emplace(GLFW_KEY_R, KeyboardInputType::SEKey_R);
		mKeyboardInputConverter.emplace(GLFW_KEY_T, KeyboardInputType::SEKey_T);
		mKeyboardInputConverter.emplace(GLFW_KEY_Y, KeyboardInputType::SEKey_Y);
		mKeyboardInputConverter.emplace(GLFW_KEY_U, KeyboardInputType::SEKey_U);
		mKeyboardInputConverter.emplace(GLFW_KEY_I, KeyboardInputType::SEKey_I);
		mKeyboardInputConverter.emplace(GLFW_KEY_O, KeyboardInputType::SEKey_O);
		mKeyboardInputConverter.emplace(GLFW_KEY_P, KeyboardInputType::SEKey_P);
		mKeyboardInputConverter.emplace(GLFW_KEY_A, KeyboardInputType::SEKey_A);
		mKeyboardInputConverter.emplace(GLFW_KEY_S, KeyboardInputType::SEKey_S);
		mKeyboardInputConverter.emplace(GLFW_KEY_D, KeyboardInputType::SEKey_D);
		mKeyboardInputConverter.emplace(GLFW_KEY_F, KeyboardInputType::SEKey_F);
		mKeyboardInputConverter.emplace(GLFW_KEY_G, KeyboardInputType::SEKey_G);
		mKeyboardInputConverter.emplace(GLFW_KEY_H, KeyboardInputType::SEKey_H);
		mKeyboardInputConverter.emplace(GLFW_KEY_J, KeyboardInputType::SEKey_J);
		mKeyboardInputConverter.emplace(GLFW_KEY_K, KeyboardInputType::SEKey_K);
		mKeyboardInputConverter.emplace(GLFW_KEY_L, KeyboardInputType::SEKey_L);
		mKeyboardInputConverter.emplace(GLFW_KEY_Z, KeyboardInputType::SEKey_Z);
		mKeyboardInputConverter.emplace(GLFW_KEY_X, KeyboardInputType::SEKey_X);
		mKeyboardInputConverter.emplace(GLFW_KEY_C, KeyboardInputType::SEKey_C);
		mKeyboardInputConverter.emplace(GLFW_KEY_V, KeyboardInputType::SEKey_V);
		mKeyboardInputConverter.emplace(GLFW_KEY_B, KeyboardInputType::SEKey_B);
		mKeyboardInputConverter.emplace(GLFW_KEY_N, KeyboardInputType::SEKey_N);
		mKeyboardInputConverter.emplace(GLFW_KEY_M, KeyboardInputType::SEKey_M);


		mMouseInputConverter.emplace(GLFW_MOUSE_BUTTON_1, MouseInputType::SEMouseLeft);
		mMouseInputConverter.emplace(GLFW_MOUSE_BUTTON_1, MouseInputType::SEMouseRight);
		mMouseInputConverter.emplace(GLFW_MOUSE_BUTTON_1, MouseInputType::SEMouseMiddle);
		mMouseInputConverter.emplace(GLFW_MOUSE_BUTTON_1, MouseInputType::SEMouseButton4);
		mMouseInputConverter.emplace(GLFW_MOUSE_BUTTON_1, MouseInputType::SEMouseButton5);

		mGamepadInputConverter.emplace(GLFW_GAMEPAD_BUTTON_A, GamepadInputType::SEGamepad_FaceButtonDown);
		mGamepadInputConverter.emplace(GLFW_GAMEPAD_BUTTON_B, GamepadInputType::SEGamepad_FaceButtonRight);
		mGamepadInputConverter.emplace(GLFW_GAMEPAD_BUTTON_X, GamepadInputType::SEGamepad_FaceButtonLeft);
		mGamepadInputConverter.emplace(GLFW_GAMEPAD_BUTTON_Y, GamepadInputType::SEGamepad_FaceButtonUp);
		mGamepadInputConverter.emplace(GLFW_GAMEPAD_BUTTON_DPAD_UP, GamepadInputType::SEGamepad_DPadUp);
		mGamepadInputConverter.emplace(GLFW_GAMEPAD_BUTTON_DPAD_DOWN, GamepadInputType::SEGamepad_DPadDown);
		mGamepadInputConverter.emplace(GLFW_GAMEPAD_BUTTON_DPAD_LEFT, GamepadInputType::SEGamepad_DPadLeft);
		mGamepadInputConverter.emplace(GLFW_GAMEPAD_BUTTON_DPAD_RIGHT, GamepadInputType::SEGamepad_DPadRight);
		mGamepadInputConverter.emplace(GLFW_GAMEPAD_BUTTON_START, GamepadInputType::SEGamepad_Start);
		mGamepadInputConverter.emplace(GLFW_GAMEPAD_BUTTON_BACK, GamepadInputType::SEGamepad_Select);
		mGamepadInputConverter.emplace(GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER, GamepadInputType::SEGamepad_RightTrigger);
		mGamepadInputConverter.emplace(GLFW_GAMEPAD_AXIS_LEFT_TRIGGER, GamepadInputType::SEGamepad_LeftTrigger);
		mGamepadInputConverter.emplace(GLFW_GAMEPAD_BUTTON_LEFT_BUMPER, GamepadInputType::SEGamepad_RightBumper);
		mGamepadInputConverter.emplace(GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER, GamepadInputType::SEGamepad_RightBumper);
		mGamepadInputConverter.emplace(GLFW_GAMEPAD_BUTTON_LEFT_THUMB, GamepadInputType::SEGamepad_LeftStickDown);
		mGamepadInputConverter.emplace(GLFW_GAMEPAD_BUTTON_RIGHT_THUMB, GamepadInputType::SEGamepad_RightStickDown);
	}

	void InputManagerGLFW::onPollInput()
	{
		swapInputBuffers();
	}
}

