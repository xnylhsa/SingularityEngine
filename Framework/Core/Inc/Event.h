#ifndef SINGULARITY_EVENT
#define SINGULARITY_EVENT

#include "Common.h"
namespace SingularityEngine::Core
{
	enum class EventType
	{
		none = 0,
		WindowClose,
		WindowResize,
		WindowFocus,
		WindowLostFocus,
		WindowMoved,
		AppTick,
		AppUpdate,
		AppRender,
		KeyPressed,
		KeyReleased,
		MouseButtonPressed,
		MouseButtonReleased,
		MouseMoved,
		MouseScrolled
	};

	enum EventCategory
	{
		None = 0,
		EventCategoryApplication = 1,
		EventCategoryInput = 2,
		EventCategoryKeyboard = 4,
		EventCategoryMouse = 8,
		EventCategoryMouseButton = 16,
		EventCategoryMaxEnum = UINT32_MAX
	};

#define EVENT_CLASS_TYPE(type) static EventType getStaticType() { return EventType::##type;} \
								virtual EventType getEventType() const override { return getStaticType();}\
								virtual const char* getName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) virtual int getCategoryFlags() const override { return category;}


	class Event
	{
		friend class EventDispatcher;
	public:
		virtual EventType getEventType() const = 0;
		virtual const char* getName() const = 0;
		virtual int getCategoryFlags() const = 0;
		virtual std::string toString() const { return getName(); }
		inline bool isInCategory(EventCategory category)
		{
			return getCategoryFlags() & category;
		}
	protected:
		bool mHandled = false;
	};

	class EventDispatcher
	{
		template<typename T>
		using EventFn = std::function<bool(T&)>;
	public:
		EventDispatcher(Event& event) : mEvent(event)
		{

		}

		template<typename T> 
		bool Dispatch(EventFn<T> func)
		{
			if (mEvent.getEventType() == T::getStaticType())
			{
				mEvent.mHandled = func(*(T*)&mEvent);
				return true;
			}
			return false;
		}
	private:
		Event& mEvent;
	};
}

#endif