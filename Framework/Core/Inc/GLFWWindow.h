#ifndef SINGULARITY_ENGINE_GLFW_WINDOW
#define SINGULARITY_ENGINE_GLFW_WINDOW
#include "Common.h"
#include "External/GLFW/include/GLFW/glfw3.h"
#include "Window.h"
namespace SingularityEngine::Core
{

	class WindowGLFW : public Window
	{

	public:

		WindowGLFW(const WindowProperties& props);
		virtual ~WindowGLFW();
		void onUpdate() override;

		inline unsigned int getWidth() const override
		{
			return mWindowData.Width;
		}

		inline unsigned int getHeight() const override
		{
			return mWindowData.Height;
		}

		inline void setEventCallback(const EventCallBackFn& callback) override
		{
			mWindowData.eventCallback = callback;
		}

		inline void setVsync(bool enabled) override
		{
			mWindowData.VSync = enabled;
		}

		inline bool isVSync() const override
		{
			return mWindowData.VSync;
		}

		inline std::string getTitle() const override { return mWindowData.Title; }


		inline void* getNativeWindow() override { return mWindow; }

	private:
		virtual void Init(const WindowProperties& props);
		virtual void Shutdown();
	private:
		GLFWwindow* mWindow;
		struct WindowData
		{
			std::string Title;
			unsigned int Width = 0, Height = 0;
			bool VSync = true;
			EventCallBackFn eventCallback = nullptr;
		};
		WindowData mWindowData;
	};


}

#endif