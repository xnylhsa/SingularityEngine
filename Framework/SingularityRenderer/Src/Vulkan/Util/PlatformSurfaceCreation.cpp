#include "precompiled.h"
#include "Vulkan/Util/PlatformSurfaceCreation.h"
#include "Vulkan/Context/VulkanContext.h"
#include "Vulkan/Util/VulkanFunctions.h"


#ifdef SINGULARITY_USE_GLFW
VkResult  SingularityEngine::SERenderer::Platform::CreateSurface(VkSurfaceKHR* surface)
{
	SingularityEngine::Core::WindowGLFW* window = (SingularityEngine::Core::WindowGLFW*)SingularityEngine::Core::Application::get()->getWindow();

	VulkanInstance* instance = VulkanContext::GetVulkanInstance();
	return glfwCreateWindowSurface(*instance, (GLFWwindow*)startupInfo.mWindow->getNativeWindow(), nullptr, surface);
}
#elif defined SINGULARITY_PLATFORM_WINDOWS
	VkResult  SingularityEngine::SERenderer::Platform::CreateSurface(VkSurfaceKHR* surface)
	{
		SingularityEngine::Core::WindowsWindow* window = (SingularityEngine::Core::WindowsWindow*)SingularityEngine::Core::Application::get()->getWindow();
		VkWin32SurfaceCreateInfoKHR surfaceCreationInfo = {
		VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR,  //sType
		nullptr,                                          //pNext
		0,                                                //Flags
		window->getInstance(),            //hInstance
		(HWND)window->getNativeWindow()            //HWND
		};

		SingularityEngine::SERenderer::VulkanInstance* instance = SingularityEngine::SERenderer::VulkanContext::GetVulkanInstance();
		return vkCreateWin32SurfaceKHR(*instance, &surfaceCreationInfo, nullptr, surface);
	}
#elif defined SINGULARITY_PLATFORM_MACOS | SINGULARITY_PLATFORM_IOS
VkResult  SingularityEngine::SERenderer::Platform::CreateSurface( VkSurfaceKHR* surface)
{
	VkMetalSurfaceCreateInfoEXT surfaceCreationInfo = {
	VK_STRUCTURE_TYPE_METAL_SURFACE_CREATE_INFO_EXT,  //sType
	nullptr,                                          //pNext
	0,                                                //Flags
	(CAMetalLayer*)window->getNativeWindow(),            //pLayer
		};

	VulkanInstance* instance = VulkanContext::GetVulkanInstance();
	return vkCreateMetalSurfaceEXT(*instance, &surfaceCreationInfo, nullptr, surface);
}
#elif defined SINGULARITY_PLATFORM_ANDROID
VkResult  SingularityEngine::SERenderer::Platform::CreateSurface(VkSurfaceKHR* surface)
{
	VkAndroidSurfaceCreateInfoKHR surfaceCreationInfo = {
	VK_STRUCTURE_TYPE_ANDROID_SURFACE_CREATE_INFO_KHR,  //sType
	nullptr,                                          //pNext
	0,                                                //Flags
	(ANativeWindow*)window->getNativeWindow(),            //pLayer
	};

	VulkanInstance* instance = VulkanContext::GetVulkanInstance();
	return vkCreateAndroidSurfaceKHR(*instance, &surfaceCreationInfo, nullptr, surface);
}
#elif defined VK_USE_PLATFORM_XLIB_KHR
VkResult  SingularityEngine::SERenderer::Platform::CreateSurface(VkSurfaceKHR* surface)
{
	Core::LinuxWindow* window = static_cast<Core:LinuxWindow>(startupInfo.mWindow);

	VkXlibSurfaceCreateInfoKHR surfaceCreationInfo = {
	VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR,  //sType
	nullptr,                                          //pNext
	0,                                                //Flags
	window->getDisplay(),
	(Window)window->getNativeWindow(),            //pLayer
	};

	VulkanInstance* instance = VulkanContext::GetVulkanInstance();
	return vkCreateXlibSurfaceKHR(*instance, &surfaceCreationInfo, nullptr, surface);
}
#elif define VK_USE_PLATFORM_XCB_KHR
VkResult  SingularityEngine::SERenderer::Platform::CreateSurface(StartupParameters startupInfo, VkSurfaceKHR* surface)
{
	Core::LinuxWindow* window = static_cast<Core:LinuxWindow>(startupInfo.mWindow);
	VkXcbSurfaceCreateInfoKHR surfaceCreationInfo = {
	VK_STRUCTURE_TYPE_XCB_SURFACE_CREATE_INFO_KHR,  //sType
	nullptr,                                          //pNext
	0,                                                //Flags
	window->getConnection(),
	(xcb_window_t)window->getNativeWindow(),            //pLayer
	};

	VulkanInstance* instance = VulkanContext::GetVulkanInstance();
	return vkCreateXcbSurfaceKHR(*instance, &surfaceCreationInfo, nullptr, surface);
}
#endif