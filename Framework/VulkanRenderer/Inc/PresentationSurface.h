#pragma once
#include "Common.h"
#include "VulkanParameters.h"
namespace SingularityEngine::Vulkan
{
	class PresentationSurface
	{
	public:
		bool init(VkInstance& instance, StartupParameters startupInfo);
		bool terminate(VkInstance& instance);
		VkSurfaceKHR& operator()() { return mPresentationSurface; }
	private:
		VkSurfaceKHR mPresentationSurface = VK_NULL_HANDLE;
	};
}

