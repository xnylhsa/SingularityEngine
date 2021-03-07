#ifndef SINGULARITY_KEY_EVENT
#define SINGULARITY_KEY_EVENT
#include "Event.h"


namespace SingularityEngine::Core
{
	class KeyEvent : public Event
	{
	public:
		inline SingularityInputType GetKeyCode() const { return mKeyCode; }
		EVENT_CLASS_CATEGORY(EventCategory::EventCategoryKeyboard | EventCategory::EventCategoryInput)
	protected:
		KeyEvent(SingularityInputType keyCode) : mKeyCode(keyCode) {}
		SingularityInputType mKeyCode = SingularityInputType::SEINPUTMAX;
	};

	class KeyPressedEvent : public KeyEvent
	{
	public:
		KeyPressedEvent(SingularityInputType keyCode) : KeyEvent(keyCode) {}
		std::string toString() const override
		{
			std::stringstream ss;
			ss << "KeyPressedEvent: " << magic_enum::enum_name(mKeyCode);
			return ss.str();
		}
		EVENT_CLASS_TYPE(KeyPressed)
	};

	class KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent(SingularityInputType keyCode) : KeyEvent(keyCode) {}
		std::string toString() const override
		{
			std::stringstream ss;
			ss << "KeyReleasedEvent: " << magic_enum::enum_name(mKeyCode);
			return ss.str();
		}
		EVENT_CLASS_TYPE(KeyReleased)
	};

}

#endif