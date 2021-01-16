#include "Precompiled.h"
#include "PresentationSurface.h"
#include "VulkanFunctions.h"
using namespace SingularityEngine::Vulkan;

bool PresentationSurface::init(VkInstance& instance, StartupParameters startupInfo)
{
	VkWin32SurfaceCreateInfoKHR surfaceCreationInfo = {
		VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR,  //sType
		nullptr,                                          //pNext
		0,                                                //Flags
		startupInfo.windowParams.mAppInstance,            //hInstance
		startupInfo.windowParams.mWindowHandle            //HWND
	};

	VkResult result = vkCreateWin32SurfaceKHR(instance, &surfaceCreationInfo, nullptr, &mPresentationSurface);
	if (result != VK_SUCCESS || mPresentationSurface == VK_NULL_HANDLE)
	{
		return false;
	}
	return true;
}

bool PresentationSurface::terminate(VkInstance& instance)
{
	if (mPresentationSurface)
	{
		LOG("[Graphics System] Destroying presentation surface");

		vkDestroySurfaceKHR(instance, mPresentationSurface, nullptr);
		mPresentationSurface = VK_NULL_HANDLE;
	}
	return true;
}
