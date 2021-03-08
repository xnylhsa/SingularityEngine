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
		int mKeyCode = -1;
	};

	class KeyPressedEvent : public KeyEvent
	{
	public:
		KeyPressedEvent(int keyCode) : KeyEvent(keyCode) {}
		std::string toString() const override
		{
			std::stringstream ss;
			ss << "KeyPressedEvent: " << mKeyCode;
			return ss.str();
		}
		EVENT_CLASS_TYPE(KeyPressed)
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