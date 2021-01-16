#include "Precompiled.h"
#include "Application.h"

using namespace SingularityEngine;
using namespace Core;


Application::Application() : mInstance(nullptr), mRunning(true)
{

}

Application::~Application()
{

}

void Application::Initialize(HINSTANCE instance, LPCSTR appName, uint32_t windowWidth, uint32_t windowHeight)
{
	mInstance = instance;
	mAppName = appName;
	HRESULT result = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
	if (result != S_OK)
	{
		return;
	}
	OnInitialize(windowWidth, windowHeight);
}
void Application::Terminate()
{
	OnTerminate();
	CoUninitialize();
}

void Application::Update()
{
	OnUpdate();
}