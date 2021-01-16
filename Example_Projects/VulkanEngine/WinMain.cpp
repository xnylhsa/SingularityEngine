#include "GameApp.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(lpCmdLine);
	UNREFERENCED_PARAMETER(nCmdShow);
	UNREFERENCED_PARAMETER(hPrevInstance);
	GameApp myApp;
	myApp.Initialize(hInstance, "Hello Vulkan!");

	while (myApp.IsRunning())
	{
		myApp.Update();
	}
	myApp.Terminate();

	return 0;
}