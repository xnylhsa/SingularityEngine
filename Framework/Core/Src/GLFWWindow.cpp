#include "precompiled.h"
#include "GLFWWindow.h"
#include "Debug.h"
namespace
{
	static bool sGLFWInitialized = false;
}


void SingularityEngine::Core::WindowGLFW::onUpdate()
{
	glfwPollEvents();
	glfwSwapBuffers(mWindow);
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

	if (!sGLFWInitialized)
	{
		int success = glfwInit();
		ASSERT((bool)success, "[Singularity::Graphics] Failed to initalize GLFW.");
		sGLFWInitialized = true;
	}

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	mWindow = glfwCreateWindow(mWindowData.Width, mWindowData.Height, mWindowData.Title.c_str(), nullptr, nullptr);
	//glfwMakeContextCurrent(mWindow);
	glfwSetWindowUserPointer(mWindow, &mWindowData);
}

void SingularityEngine::Core::WindowGLFW::Shutdown()
{
	if (mWindow)
	{
		glfwDestroyWindow(mWindow);
	}
}