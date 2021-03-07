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
		Application(const Application&) = delete;
		Application& operator=(const Application&) = delete;
		void Initialize(WindowProperties windowProps);
		void Terminate();

		void Update();
		void Run();

		void OnEvent(Event& e);

		Core::Window* getWindow() { return mWindow.get(); }
		inline bool IsRunning() const { return mRunning; }
		inline bool IsFocused() const { return mFocused; }
		void RegisterEventFunc(Core::EventType type, Core::EventDispatcher::EventFn func) { mEventManager.registerDispatchFunction(type, func); }
	protected:
		inline void Kill() { mRunning = false; }
		bool OnWindowClose(Core::Event& e);
		bool OnWindowLostFocus(Core::Event& e);
		bool OnWindowFocus(Core::Event& e);
		bool OnWindowResize(Core::Event& e);

		Application();
		virtual ~Application();
		virtual void OnInitialize() = 0;
		virtual void OnTerminate() = 0;
		virtual void OnUpdate() = 0;
	private:
		std::unique_ptr<Core::Window> mWindow;
		EventManager mEventManager;
		bool mRunning;
		bool mFocused = true;
	};

}// namespace SingularityEngine::Core



#endif //INCLUDED_CORE_APPLICATION_H
