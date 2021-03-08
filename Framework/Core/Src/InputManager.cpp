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
		return sInstance->onInitalize();
	}

	bool InputManager::Teardown()
	{
		ASSERT((sInstance->mInitialized), "[Core::Input] Cannot teardown input system until it is initalized!");
		if (!sInstance->mInitialized) return false;
		sInstance->mGamepadInputConverter.clear();
		sInstance->mMouseInputConverter.clear();
		sInstance->mKeyboardInputConverter.clear();
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

}