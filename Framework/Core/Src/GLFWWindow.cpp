#include "precompiled.h"
#include "GLFWWindow.h"
#include "Debug.h"
#include "ApplicationEvent.h"
#include "MouseEvent.h"
#include "KeyEvent.h"
namespace
{
	static bool sGLFWInitialized = false;

	static void GLFWErrorCallback(int error, const char* description)
	{
		LOG("[Core::GLFW] Error %d, %s", error, description);
	}
}


void SingularityEngine::Core::WindowGLFW::onUpdate()
{
	glfwPollEvents();
	//glfwSwapBuffers(mWindow);
}

SingularityEngine::Core::WindowGLFW::WindowGLFW(const WindowProperties& props)
{
	Init(props);
}

SingularityEngine::Core::WindowGLFW::~WindowGLFW()
{
	Shutdown();
}

void SingularityEngine::Core::WindowGLFW::Init(const WindowProperties& props)
{
	mWindowData.Title = props.mTitle;
	mWindowData.Height = props.mHeight;
	mWindowData.Width = props.mWidth;
	mWindowData.VSync = props.useVync;
	mWindowData.eventCallback = nullptr;
	mWindowData.inputConverter.initialize();
	if (!sGLFWInitialized)
	{
		int success = glfwInit();
		ASSERT((bool)success, "[Singularity::Graphics] Failed to initalize GLFW.");
		glfwSetErrorCallback(GLFWErrorCallback);
		sGLFWInitialized = true;
	}

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	mWindow = glfwCreateWindow(mWindowData.Width, mWindowData.Height, mWindowData.Title.c_str(), nullptr, nullptr);

	//glfwMakeContextCurrent(mWindow);
	glfwSetWindowUserPointer(mWindow, &mWindowData);
	glfwSetWindowSizeCallback(mWindow, [](GLFWwindow* window, int width, int height)
	{
			 WindowData& data = *(WindowData*)(glfwGetWindowUserPointer(window));
			 data.Width = width;
			 data.Height = height;
			 WindowResizeEvent resizeEvent(width, height);
			 data.eventCallback(resizeEvent);
	});
	glfwSetWindowCloseCallback(mWindow, [](GLFWwindow* window)
	{
			WindowData& data = *(WindowData*)(glfwGetWindowUserPointer(window));
			WindowCloseEvent closeEvent;
			data.eventCallback(closeEvent);
	});

	glfwSetKeyCallback(mWindow, [](GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		UNREFERENCED_PARAMETER(scancode);
		UNREFERENCED_PARAMETER(action);
		UNREFERENCED_PARAMETER(mods);

		WindowData& data = *(WindowData*)(glfwGetWindowUserPointer(window));
		switch (action)
		{
			case GLFW_PRESS:
			{
				SingularityInputType inputType = data.inputConverter.convert(key);
				KeyPressedEvent keyEvent(inputType);
				data.eventCallback(keyEvent);
				break;
			}
			case GLFW_RELEASE:
			{
				SingularityInputType inputType = data.inputConverter.convert(key);
				KeyReleasedEvent keyEvent(inputType);
				data.eventCallback(keyEvent);
				break;
			}
			case GLFW_REPEAT:
			{
				SingularityInputType inputType = data.inputConverter.convert(key);
				KeyPressedEvent keyEvent(inputType);
				data.eventCallback(keyEvent);
				break;
			}
		}
	});

	glfwSetMouseButtonCallback(mWindow, [](GLFWwindow* window, int button, int action, int mods)
	{
		UNREFERENCED_PARAMETER(action);
		UNREFERENCED_PARAMETER(mods);
		WindowData& data = *(WindowData*)(glfwGetWindowUserPointer(window));
		switch (action)
		{
			case GLFW_PRESS:
			{
				SingularityInputType inputType = data.inputConverter.convert(button);
				MousePressedEvent mouseEvent(inputType);
				data.eventCallback(mouseEvent);
				break;
			}
			case GLFW_RELEASE:
			{
				SingularityInputType inputType = data.inputConverter.convert(button);
				MouseReleasedEvent mouseEvent(inputType);
				data.eventCallback(mouseEvent);
				break;
			}
		}
	});

	glfwSetScrollCallback(mWindow, [](GLFWwindow* window, double xOffset, double yOffset)
	{
		WindowData& data = *(WindowData*)(glfwGetWindowUserPointer(window));
		Core::MouseScrolledEvent scrolledEvent((float)xOffset, (float)yOffset);
		data.eventCallback(scrolledEvent);
	});

	glfwSetCursorPosCallback(mWindow, [](GLFWwindow* window, double xPos, double yPos)
	{
		WindowData& data = *(WindowData*)(glfwGetWindowUserPointer(window));
		Core::MouseMovedEvent movedEvent((float)xPos, (float)yPos);
		data.eventCallback(movedEvent);
	});

	glfwSetWindowFocusCallback(mWindow, [](GLFWwindow* window, int focused)
	{
		WindowData& data = *(WindowData*)(glfwGetWindowUserPointer(window));
		if (focused == GLFW_TRUE)
		{
			WindowFocusEvent focusEvent;
			data.eventCallback(focusEvent);
		}
		else
		{
			WindowLostFocusEvent lostFocusEvent;
			data.eventCallback(lostFocusEvent);
		}
	});

}

void SingularityEngine::Core::WindowGLFW::Shutdown()
{
	if (mWindow)
	{
		glfwDestroyWindow(mWindow);
	}
}
