#ifndef INCLUDED_CORE_APPLICATION_H
#define INCLUDED_CORE_APPLICATION_H
#include "Window.h"
#include "Event.h"
#include "ApplicationEvent.h"

namespace SingularityEngine::Core 
{

	class Application
	{
	public:

		static bool create(WindowProperties windowProps, Application* appPtr);
		static void destroy();
		static Application* get();
		Application(const Application&) = delete;
		Application& operator=(const Application&) = delete;

		void update();
		void run();

		void onEvent(Event& e);

		Core::Window* getWindow() { return mWindow.get(); }
		inline bool isRunning() const { return mRunning; }
		inline bool isFocused() const { return mFocused; }
		void registerEventFunc(Core::EventType type, Core::EventDispatcher::EventFn func) { mEventManager.registerDispatchFunction(type, func); }
	protected:
		inline void kill() { mRunning = false; }
		bool onWindowClose(Core::Event& e);
		bool onWindowLostFocus(Core::Event& e);
		bool onWindowFocus(Core::Event& e);

		Application();
		virtual ~Application();
		virtual void onInitialize() = 0;
		virtual void onTerminate() = 0;
		virtual void onUpdate() = 0;
	private:
		void initialize(WindowProperties windowProps);
		void terminate();
		static Application* sInstance;
		std::unique_ptr<Core::Window> mWindow;
		EventManager mEventManager;
		bool mRunning;
		bool mFocused = true;
	};

}// namespace SingularityEngine::Core



#endif //INCLUDED_CORE_APPLICATION_H
