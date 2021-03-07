#ifndef SINGULARITY_EVENT
#define SINGULARITY_EVENT

#include "Common.h"
#include "SEInputTypes.h"
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
	public:
		using EventFn = std::function<bool(Event&)>;
		void pushDispatchFunction(EventFn& eventFN)
		{
			mEventFNs.push_back(eventFN);
		}

		bool Dispatch(Event& e)
		{
			size_t numEventFNs = mEventFNs.size();
			for (size_t i =0; i < numEventFNs; ++i)
			{
				e.mHandled = mEventFNs[i](*(Event*)&e);
				if (e.mHandled)
					return true;
			}
			return e.mHandled;
		}
	private:
		std::vector<EventFn> mEventFNs;
	};

	class EventManager
	{
	public:
		void registerDispatchFunction(EventType type, EventDispatcher::EventFn dispatchFunc);
		bool processEvent(Event& e);

	private:
		std::map<EventType, EventDispatcher> mEventDispatchers;
	};

#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

}

#endif