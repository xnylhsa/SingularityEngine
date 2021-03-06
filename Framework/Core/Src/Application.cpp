#include "Precompiled.h"
#include "Application.h"
#include "Debug.h"
using namespace SingularityEngine;
using namespace Core;


Application::Application(): mRunning(true)
{
}

Application::~Application()
{
}

void Application::Initialize(WindowProperties windowProps)
{
	mWindow = Window::create(windowProps);
	OnInitialize();
}

void Application::Terminate()
{
	OnTerminate();
	mWindow.reset();
	mWindow = nullptr;
}

void Application::Update()
{
	OnUpdate();
}

void Application::Run()
{

}
