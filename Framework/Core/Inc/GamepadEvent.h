#ifndef SINGULARITY_GAMEPAD_EVENT
#define SINGULARITY_GAMEPAD_EVENT
#include "Event.h"


namespace SingularityEngine::Core
{
	class GamepadEvent : public Event
	{
	public:
		inline int getButton() const { return mButton; }
		inline int getGamepadIndex() const { return mGamepadIndex; }
		EVENT_CLASS_CATEGORY(EventCategory::EventCategoryGamepadButton | EventCategory::EventCategoryInput)
	protected:
		GamepadEvent(int button, int gamepadIndex) : mButton(button), mGamepadIndex(gamepadIndex) {}
		int mGamepadIndex = -1;
		int mButton = -1;
	};

	class GamepadButtonPressedEvent : public GamepadEvent
	{
	public:
		GamepadButtonPressedEvent(int button, int gamepadIndex) : GamepadEvent(button, gamepadIndex) {}
		std::string toString() const override
		{
			std::stringstream ss;
			ss << "GamepadButtonPressedEvent: " << mButton << " From input device index: " << mGamepadIndex;
			return ss.str();
		}
		EVENT_CLASS_TYPE(GamepadButtonPressed)
	};

	class GamepadButtonReleasedEvent : public GamepadEvent
	{
	public:
		GamepadButtonReleasedEvent(int button, int gamepadIndex) : GamepadEvent(button, gamepadIndex) {}
		std::string toString() const override
		{
			std::stringstream ss;
			ss << "GamepadButtonReleasedEvent: " << mButton << " From input device index: " << mGamepadIndex;
			return ss.str();
		}
		EVENT_CLASS_TYPE(GamepadButtonReleased)
	};

	class GamepadJoystickMovedEvent : public Event
	{
	public:

		GamepadJoystickMovedEvent(float x, float y, int gamepadIndex, bool isLeftStick) : mPosX(x), mPosY(y), mGamepadIndex(gamepadIndex), mIsLeftStick(isLeftStick) {}
		std::string toString() const override
		{
			std::stringstream ss;
			ss << "GamepadJoystickMovedEvent: X:" << mPosX << " Y:" << mPosY << " on " << (mIsLeftStick ? "left joystick" : "right joystick") << " for gamepad index: " << mGamepadIndex;
			return ss.str();
		}
		inline float getX() { return mPosX; }
		inline float getY() { return mPosY; }
		inline bool isLeftStick() { return mIsLeftStick; }
		inline int getGamepadIndex() { return mGamepadIndex; }
		EVENT_CLASS_TYPE(GamepadJoystickMoved)
		EVENT_CLASS_CATEGORY(EventCategoryGamepad | EventCategoryInput)
	private:
		int mGamepadIndex;
		bool mIsLeftStick;
		float mPosX;
		float mPosY;
	};


}

#endif