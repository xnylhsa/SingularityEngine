#ifndef SINGULARITY_APPLICATION_EVENT
#define SINGULARITY_APPLICATION_EVENT
#include "Event.h"

namespace SingularityEngine::Core
{
	class WindowResizeEvent : public Event {
	public:
		class WindowResizeEvent(unsigned int width, unsigned int height) : mWidth(width), mHeight(height) {}
		inline unsigned int getWidth() const { return mWidth; }
		inline unsigned int getHeight() const { return mHeight; }
		std::string toString() const override
		{
			std::stringstream ss;
			ss << "WindowResizeEvent: \nWidth:" << mWidth << ", \nHeight:" << mHeight;
			return ss.str();
		}

		EVENT_CLASS_TYPE(WindowResize)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	private:
		unsigned int mWidth;
		unsigned int mHeight;
	};

	class WindowCloseEvent : public Event {
	public:
		class WindowCloseEvent() {}
		std::string toString() const override
		{
			std::stringstream ss;
			ss << "WindowCloseEvent";
			return ss.str();
		}

		EVENT_CLASS_TYPE(WindowClose)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class AppTickEvent : public Event {
	public:
		class AppTickEvent() {}
		std::string toString() const override
		{
			std::stringstream ss;
			ss << "AppTickEvent";
			return ss.str();
		}

		EVENT_CLASS_TYPE(AppTick)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class AppUpdateEvent : public Event {
	public:
		class AppUpdateEvent() {}
		std::string toString() const override
		{
			std::stringstream ss;
			ss << "AppUpdateEvent";
			return ss.str();
		}

		EVENT_CLASS_TYPE(AppUpdate)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class AppRenderEvent : public Event {
	public:
		class AppRenderEvent() {}
		std::string toString() const override
		{
			std::stringstream ss;
			ss << "AppRenderEvent";
			return ss.str();
		}

		EVENT_CLASS_TYPE(AppRender)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};


}

#endif