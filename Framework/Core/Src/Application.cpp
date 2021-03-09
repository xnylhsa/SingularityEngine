#include "Precompiled.h"
#include "Application.h"
#include "Debug.h"
#include "DeleteUtil.h"
#include "InputManager.h"



SingularityEngine::Core::Application* SingularityEngine::Core::Application::sInstance = nullptr;

using namespace SingularityEngine;
using namespace Core;


Application::Application(): mRunning(true)
{
}

Application::~Application()
{
}

void Application::initialize(WindowProperties windowProps, AppVer appVersion)
{
	mAppVersion = appVersion;
	mWindow = Window::create(windowProps);
	mWindow->setEventCallback(BIND_EVENT_FN(Application::onEvent));
	mEventManager.registerDispatchFunction(Core::EventType::WindowClose, BIND_EVENT_FN(Application::onWindowClose));
	mEventManager.registerDispatchFunction(Core::EventType::WindowLostFocus, BIND_EVENT_FN(Application::onWindowLostFocus));
	mEventManager.registerDispatchFunction(Core::EventType::WindowFocus, BIND_EVENT_FN(Application::onWindowFocus));
	InputManager::Initalize();
	onInitialize();
}

void Application::terminate()
{
	onTerminate();
	InputManager::Teardown();
	mWindow.reset();
	mWindow = nullptr;
}

void Application::update()
{
	InputManager::pollInput();
	mWindow->onUpdate();
	if (Core::InputManager::isKeyPressed(Core::KeyboardInputType::SEKey_Escape))
	{
		kill();
		return;
	}
	onUpdate();
}

void Application::run()
{

}

void Application::onEvent(Event& e)
{
	mEventManager.processEvent(e);
	//LOG("%s", e.toString().c_str());
}

bool Application::onWindowClose(Core::Event& e)
{
	WindowCloseEvent& closeEvent = *dynamic_cast<WindowCloseEvent*>(&e);
	UNREFERENCED_PARAMETER(closeEvent);
	kill();
	return true;
}

bool Application::onWindowLostFocus(Core::Event& e)
{
	WindowLostFocusEvent& event = *dynamic_cast<WindowLostFocusEvent*>(&e);
	UNREFERENCED_PARAMETER(event);
	mFocused = false;
	return true;
}

bool Application::onWindowFocus(Core::Event& e)
{
	WindowFocusEvent& event = *dynamic_cast<WindowFocusEvent*>(&e);
	UNREFERENCED_PARAMETER(event);
	mFocused = true;
	return true;
}

bool Application::create(WindowProperties windowProps, AppVer appVer, Application* appPtr)
{
	ASSERT(sInstance == nullptr, "[Core::App] app instance already created!");
	if (sInstance != nullptr) return false;
	sInstance = appPtr;
	appPtr = nullptr;
	sInstance->initialize(windowProps, appVer);
	return true;
}

void Application::destroy()
{
	sInstance->terminate();
	delete sInstance;
	sInstance = nullptr;
}

SingularityEngine::Core::Application* Application::get()
{
	ASSERT(sInstance != nullptr, "[Core::App] App instance was never created!");
	return sInstance;
}
