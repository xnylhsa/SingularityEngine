#ifndef INCLUDED_CORE_APPLICATION_H
#define INCLUDED_CORE_APPLICATION_H

namespace SingularityEngine::Core 
{
	class Application
	{
	public:
		Application();
		virtual ~Application();

		Application(const Application&) = delete;
		Application& operator=(const Application&) = delete;

		void Initialize(HINSTANCE instance, LPCSTR appName, uint32_t windowWidth = 1920, uint32_t windowHeight = 1080);

		void Terminate();

		void Update();

		void Kill() { mRunning = false; }

		HINSTANCE GetInstance() const {return mInstance;}
		const char* GetAppName() const { return mAppName.c_str(); }
		bool IsRunning() const { return mRunning; }
		protected:
		virtual void OnInitialize(uint32_t w, uint32_t h) = 0;
		virtual void OnTerminate() = 0;
		virtual void OnUpdate() = 0;

		private:
		HINSTANCE mInstance;
		//HWND mWindow;
		std::string mAppName;
		bool mRunning;
	};

}// namespace SingularityEngine::Core



#endif //INCLUDED_CORE_APPLICATION_H
