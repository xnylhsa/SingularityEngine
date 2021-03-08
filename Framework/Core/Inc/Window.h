#ifndef INCLUDED_CORE_WINDOW_H
#define INCLUDED_CORE_WINDOW_H
#include "Common.h"
#include "Event.h"
namespace SingularityEngine::Core {

		struct WindowProperties
		{
			std::string mTitle;
			unsigned int mWidth;
			unsigned int mHeight;
			bool useVync;
			WindowProperties(const std::string& title = "Singularity Engine", unsigned int width = 1280, unsigned int height = 720, bool vSyncEnabled = true) : mTitle(title), mWidth(width), mHeight(height), useVync(vSyncEnabled) {};
		};


		class Window
		{
		public:
			using EventCallBackFn = std::function<void(Event&)>;
			virtual ~Window() = default;
			virtual void onUpdate() = 0;
			virtual unsigned int getWidth() const = 0;
			virtual unsigned int getHeight() const = 0;
			virtual std::string getTitle() const = 0;
			virtual void setEventCallback(const EventCallBackFn& callback) = 0;
			virtual void setVsync(bool enabled) = 0;
			virtual bool isVSync() const = 0;
			virtual void* getNativeWindow() = 0;
			static std::unique_ptr<Window> create(const WindowProperties& props = WindowProperties());
		private:
		};

}
#endif // !INCLUDED_CORE_WINDOW_H


