#ifndef SINGULARITY_MOUSE_EVENT
#define SINGULARITY_MOUSE_EVENT
#include "Event.h"

namespace SingularityEngine::Core
{

	class MouseMovedEvent : public Event
	{
	public:
		MouseMovedEvent(float x, float y) :deltaX(x), deltaY(y){}
		std::string toString() const override
		{
			std::stringstream ss;
			ss << "MouseMovedEvent: X:" << deltaX << " Y:" << deltaY;
			return ss.str();
		}
		inline float getX() { return deltaX; }
		inline float getY() { return deltaY; }

		EVENT_CLASS_TYPE(MouseMoved)
		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
	private:
		float deltaX;
		float deltaY;
	};

	class MouseScrolledEvent : public Event
	{
	public:
		MouseScrolledEvent(float xOffset, float yOffset) :deltaX(xOffset), deltaY(yOffset) {}
		std::string toString() const override
		{
			std::stringstream ss;
			ss << "MouseScrolledEvent: X:" << deltaX << " Y:" << deltaY;
			return ss.str();
		}
		inline float getX() { return deltaX; }
		inline float getY() { return deltaY; }

		EVENT_CLASS_TYPE(MouseScrolled)
		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
	private:
		float deltaX;
		float deltaY;
	};

	class MouseButtonEvent : public Event
	{
	public:
		inline int getMouseButton() { return mButton; }
		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
	protected:
		MouseButtonEvent(int button) : mButton(button) {}
		int mButton;
	};

	class MousePressedEvent : public MouseButtonEvent
	{
	public:
		MousePressedEvent(int button) : MouseButtonEvent(button) {}
		std::string toString() const override
		{
			std::stringstream ss;
			ss << "MousePressedEvent: " << mButton;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseButtonPressed)
	};

	class MouseReleasedEvent : public MouseButtonEvent
	{
	public:
		MouseReleasedEvent(int button) : MouseButtonEvent(button) {}
		std::string toString() const override
		{
			std::stringstream ss;
			ss << "MouseReleasedEvent: " << mButton;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseButtonReleased)
	};

}

#endif