#include "precompiled.h"
#include "InputManager.h"
#include "MemoryUtil.h"
#include "Debug.h"

namespace SingularityEngine::Core
{

	bool InputManager::isKeyPressedPlatformImpl(KeyboardInputType input) const
	{
		return mPressedKeys[(size_t)input];
	}

	bool InputManager::isMouseButtonPressedPlatformImpl(MouseInputType input) const
	{
		return mCurrentMouseButtons[(size_t)input];
	}

	float InputManager::getMouseXPlatformImpl() const
	{
		return mMouseScreenPositionX;
	}

	float InputManager::getMouseYPlatformImpl() const
	{
		return mMouseScreenPositionY;
	}

	float InputManager::getMouseDeltaXPlatformImpl() const
	{
		return mMouseDeltaX;
	}

	float InputManager::getMouseDeltaYPlatformImpl() const
	{
		return mMouseDeltaY;
	}

	bool InputManager::isMouseLeftEdgePlatformImpl() const
	{
		return mMouseLeftEdge;
	}

	bool InputManager::isMouseRightEdgePlatformImpl() const
	{
		return mMouseRightEdge;
	}

	bool InputManager::isMouseTopEdgePlatformImpl() const
	{
		return mMouseTopEdge;
	}

	bool InputManager::isMouseBottomEdgePlatformImpl() const
	{
		return mMouseBottomEdge;
	}

	bool InputManager::isGamepadButtonPressedPlatformImpl(GamepadInputType input)
	{
		bool current = mCurrentGamepadButtons[(size_t)input];
		bool last = mLastGamepadButtons[(size_t)input];
		return !last && current;
	}

	bool InputManager::isGamepadButtonDownPlatformImpl(GamepadInputType input)
	{
		return mCurrentGamepadButtons[(size_t)input];
	}

	bool InputManager::Initalize()
	{
		ASSERT(!(sInstance->mInitialized), "[Core::Input] instance for input already initalized!");
		if (sInstance->mInitialized) return false;
		sInstance->registerKeyMappings();
		sInstance->registerEventBindings();
		sInstance->mInitialized = true;
		return sInstance->onInitalize();
	}

	bool InputManager::Teardown()
	{
		ASSERT((sInstance->mInitialized), "[Core::Input] Cannot teardown input system until it is initalized!");
		if (!sInstance->mInitialized) return false;
		sInstance->mGamepadInputConverter.clear();
		sInstance->mMouseInputConverter.clear();
		sInstance->mKeyboardInputConverter.clear();
		sInstance->mInitialized = false;
		return sInstance->onTeardown();
	}

	InputManager* InputManager::get()
	{
		ASSERT(sInstance != nullptr, "[Core::Input] InputManager is not setup yet!");
		return sInstance;
	}

	void InputManager::pollInput()
	{
		sInstance->onPollInput();
	}

	bool InputManager::isKeyHeldPlatformImpl(KeyboardInputType input) const
	{
		return mCurrentKeyStateBuffer[(size_t)input];
	}

	void InputManager::swapInputBuffers()
	{
		// Store the previous keyboard state
		auto sizeOfKeyBuffer = (size_t)KeyboardInputType::KEYBOARD_INPUT_MAX;
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

}