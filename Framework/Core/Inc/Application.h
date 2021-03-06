#ifndef INCLUDED_CORE_APPLICATION_H
#define INCLUDED_CORE_APPLICATION_H
#include "Window.h"

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
		void Kill() { mRunning = false; }

		Core::Window* getWindow() { return mWindow.get(); }
		bool IsRunning() const { return mRunning; }
	protected:
		Application();
		virtual ~Application();
		virtual void OnInitialize() = 0;
		virtual void OnTerminate() = 0;
		virtual void OnUpdate() = 0;
	private:
		std::unique_ptr<Core::Window> mWindow;
		bool mRunning;
		
	};

}// namespace SingularityEngine::Core



#endif //INCLUDED_CORE_APPLICATION_H
