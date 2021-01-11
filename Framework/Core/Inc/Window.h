#ifndef INCLUDED_CORE_WINDOW_H
#define INCLUDED_CORE_WINDOW_H
#include "Common.h"
namespace SingularityEngine {
	namespace Core {

		class Window
		{
		public:
			Window();
			~Window();
			void Initialize(HINSTANCE instance, LPTSTR appName, uint32_t width, uint32_t height,WNDPROC wndProcFunction);

			void Initialize(HINSTANCE instance, LPCSTR appName, uint32_t width, uint32_t height);
			void Terminate();
			bool ProcessMessage();

			HWND GetWindowHandle() const { return mWindow; }
		private:

			HINSTANCE mInstance;
			HWND mWindow;
			std::string mAppName;
		};

	}
}
#endif // !INCLUDED_CORE_WINDOW_H


