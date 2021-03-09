#include "GameApp.h"
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(lpCmdLine);
	UNREFERENCED_PARAMETER(nCmdShow);
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(hInstance);
	Core::WindowProperties props;
	props.useVync = true;
	props.mTitle = "Hello Vulkan";
	Core::Application::create(props, {0, 1, 1}, new GameApp());
	{
		auto* myApp = dynamic_cast<GameApp*>(Core::Application::get());
		while (myApp->isRunning())
		{
			myApp->update();
		}
	}
	Core::Application::destroy();
	return 0;
}