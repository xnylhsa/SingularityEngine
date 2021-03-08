#ifndef SINGULARITY_ENGINE_WINDOWS_WINDOW
#define SINGULARITY_ENGINE_WINDOWS_WINDOW
#include "Common.h"
#include "Window.h"
#include "ApplicationEvent.h"
#include "MouseEvent.h"
#include "KeyEvent.h"

namespace SingularityEngine::Core
{

	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowProperties& properties);
		~WindowsWindow();
		std::string getName() const { return mAppName; }
		void onUpdate() override;


		inline unsigned int getWidth() const override
		{
			return mWidth;
		}
		inline std::string getTitle() const override { return mAppName; }


		inline unsigned int getHeight() const override
		{
			return mHeight;
		}


		inline void setEventCallback(const EventCallBackFn& callback) override
		{
			mEventCallback = callback;
		}


		inline void setVsync(bool enabled) override
		{
			mIsVSync = enabled;
		}


		inline virtual bool isVSync() const override
		{
			return mIsVSync;
		}

		inline HINSTANCE getInstance() { return mInstance; }

		inline void* getNativeWindow() override { return mWindow; }
		void propigateEvent(Event& event);

		bool isCursorLocked() override;


		void setCursorLocked(bool isLocked) override;


		bool clipCursorToWindow() override;


		void setClipCursor(bool isClipped) override;


		bool useRawInput() override;


		void setUseRawInput(bool useRawInput) override;

	private:
		void Initialize(HINSTANCE instance, WNDPROC wndProcFunction);

		void Initialize(HINSTANCE instance);
		void Terminate();
		bool ProcessMessage();
		bool isMinimized();
		bool mIsVSync = false;
		int mWidth;
		int mHeight;
		std::string mAppName;
		EventCallBackFn mEventCallback;
		HINSTANCE mInstance;
		HWND mWindow;
		bool mIsCursorLocked = false;
		bool mShouldClipCursor = false;
		bool mUseRawInput = false;
	};
}

#endif