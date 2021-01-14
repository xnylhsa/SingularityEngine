#include "../Framework/Core/Inc/Core.h"
#include "../Framework/Math/Inc/EngineMath.h"
#include "../Framework/PhysicsMath/Inc/PhysicsMath.h"
#include "../Framework/VulkanRenderer/Inc/VulkanRenderer.h"
#include "External/vulkan/vulkan_win32.h"

int main()
{
	SingularityEngine::Vulkan::GraphicsSystem gs;
	SingularityEngine::Vulkan::StartupParameters startupInfo;

	startupInfo.mDeviceParameters.mDesiredDeviceExtensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
	startupInfo.mInstanceParameters.mDesiredExtensions.push_back(VK_KHR_SURFACE_EXTENSION_NAME);
#if defined(_WIN32) || (_WIN64)
	startupInfo.mInstanceParameters.mDesiredExtensions.push_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
#elif defined(VK_USE_PLATFORM_XCB_KHR)
	startupInfo.mInstanceParameters.mDesiredExtensions.push_back(VK_KHR_XCB_SURFACE_EXTENSION_NAME);
#elif defined(VK_USE_PLATFORM_XLIB_KHR)
	startupInfo.mInstanceParameters.mDesiredExtensions.push_back(VK_KHR_XLIB_SURFACE_EXTENSION_NAME);
#endif
	startupInfo.mDeviceParameters.mDesiredFeatures.geometryShader = VK_TRUE;

	startupInfo.mAppName = "Vulkan Test";
	startupInfo.mMajorVersion = 0;
	startupInfo.mMinorVersion = 1;
	startupInfo.mPatchVersion = 1;

	startupInfo.mEngineName = "Singularity Engine";
	startupInfo.mEngineMajorVersion = 0;
	startupInfo.mEngineMinorVersion = 1;
	startupInfo.mEnginePatchVersion = 1;

	startupInfo.mQueueParameters.push_back({ UINT32_MAX, VK_QUEUE_COMPUTE_BIT , { {1.0f} } });
	startupInfo.mQueueParameters.push_back({ UINT32_MAX, VK_QUEUE_GRAPHICS_BIT , { {1.0f} } });
	//startupInfo.mQueueParameters.push_back({ UINT32_MAX, VK_QUEUE_TRANSFER_BIT , { {1.0f} } });




	gs.init(startupInfo);
	gs.shutdown();
}