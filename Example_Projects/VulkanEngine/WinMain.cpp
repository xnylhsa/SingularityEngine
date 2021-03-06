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
	GameApp myApp;
	myApp.Initialize(props);
	while (myApp.IsRunning())
	{
		myApp.Update();
	}
	myApp.Terminate();
	return 0;
}