#ifndef INCLUDED_CORE_APPLICATION_H
#define INCLUDED_CORE_APPLICATION_H

#include "Window.h"
#include "Event.h"
#include "ApplicationEvent.h"

namespace SingularityEngine::Core 
{

	struct AppVer
	{
		uint32_t majorVersion = 0, minorVersion = 0, patchVersion = 0;
	};

	class Application
	{
	public:

		static bool create(WindowProperties windowProps, AppVer appVer, Application* appPtr);
		static void destroy();
		static Application* get();
		Application(const Application&) = delete;
		Application& operator=(const Application&) = delete;

		void update();
		void run();

		void onEvent(Event& e);

		Core::Window* getWindow() { return mWindow.get(); }
		AppVer getVersion() { return mAppVersion; }
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
		void initialize(WindowProperties windowProps, AppVer appVersion);
		void terminate();
		static Application* sInstance;
		std::unique_ptr<Core::Window> mWindow;
		EventManager mEventManager;
		bool mRunning;
		bool mFocused = true;
		AppVer mAppVersion;
	};

}// namespace SingularityEngine::Core



#endif //INCLUDED_CORE_APPLICATION_H
