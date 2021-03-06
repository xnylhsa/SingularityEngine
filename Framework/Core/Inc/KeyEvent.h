#ifndef SINGULARITY_KEY_EVENT
#define SINGULARITY_KEY_EVENT
#include "Event.h"

namespace SingularityEngine::Core
{
	class KeyEvent : public Event
	{
	public:
		inline int GetKeyCode() const { return mKeyCode; }
		EVENT_CLASS_CATEGORY(EventCategory::EventCategoryKeyboard | EventCategory::EventCategoryInput)
	protected:
		KeyEvent(int keyCode) : mKeyCode(keyCode) {}
		int mKeyCode = INT_MAX;
	};

	class KeyPressedEvent : public KeyEvent
	{
	public:
		KeyPressedEvent(int keyCode, int repeatCount) : KeyEvent(keyCode), mRepeatCount(repeatCount) {}
		inline int GetRepeatCount() const { return mRepeatCount; }
		std::string toString() const override
		{
			std::stringstream ss;
			ss << "KeyPressedEvent: " << mKeyCode << " (" << mRepeatCount << " repeats)";
			return ss.str();
		}
		EVENT_CLASS_TYPE(KeyPressed)
	private:
		int mRepeatCount = INT_MAX;
	};

	class KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent(int keyCode) : KeyEvent(keyCode) {}
		std::string toString() const override
		{
			std::stringstream ss;
			ss << "KeyReleasedEvent: " << mKeyCode;
			return ss.str();
		}
		EVENT_CLASS_TYPE(KeyReleased)
	};

}

#endif