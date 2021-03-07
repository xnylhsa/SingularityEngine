#include "precompiled.h"
#include "Event.h"

void SingularityEngine::Core::EventManager::registerDispatchFunction(EventType type, EventDispatcher::EventFn dispatchFunc)
{
	const auto& eventDispatcher = mEventDispatchers.find(type);
	if (eventDispatcher != mEventDispatchers.end())
	{
		eventDispatcher->second.pushDispatchFunction(dispatchFunc);
	}
	else
	{
		EventDispatcher dispatcher;
		dispatcher.pushDispatchFunction(dispatchFunc);
		mEventDispatchers.emplace(type, dispatcher);
	}
}

bool SingularityEngine::Core::EventManager::processEvent(Event& e)
{
	const auto& eventDispatcher = mEventDispatchers.find(e.getEventType());
	if (eventDispatcher != mEventDispatchers.end())
	{
		return eventDispatcher->second.Dispatch(e);
	}
	return false;
}
