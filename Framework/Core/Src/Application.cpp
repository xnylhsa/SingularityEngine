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
	mWindow->setEventCallback(BIND_EVENT_FN(Application::OnEvent));
	mEventManager.registerDispatchFunction(Core::EventType::WindowClose, BIND_EVENT_FN(Application::OnWindowClose));
	mEventManager.registerDispatchFunction(Core::EventType::WindowLostFocus, BIND_EVENT_FN(Application::OnWindowLostFocus));
	mEventManager.registerDispatchFunction(Core::EventType::WindowFocus, BIND_EVENT_FN(Application::OnWindowFocus));
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
	mWindow->onUpdate();
	OnUpdate();
}

void Application::Run()
{

}

void Application::OnEvent(Event& e)
{
	mEventManager.processEvent(e);
	//EventDispatcher dispatcher(e);


	LOG("%s",e.toString().c_str());
}

bool Application::OnWindowClose(Core::Event& e)
{
	WindowCloseEvent& closeEvent = *dynamic_cast<WindowCloseEvent*>(&e);
	UNREFERENCED_PARAMETER(closeEvent);
	Kill();
	return true;
}

bool Application::OnWindowLostFocus(Core::Event& e)
{
	WindowLostFocusEvent& event = *dynamic_cast<WindowLostFocusEvent*>(&e);
	UNREFERENCED_PARAMETER(event);
	mFocused = false;
	return true;
}

bool Application::OnWindowFocus(Core::Event& e)
{
	WindowFocusEvent& event = *dynamic_cast<WindowFocusEvent*>(&e);
	UNREFERENCED_PARAMETER(event);
	mFocused = true;
	return true;
}

bool Application::OnWindowResize(Core::Event& e)
{
	UNREFERENCED_PARAMETER(e);

	//mWindow->onResize
	return true;
}
