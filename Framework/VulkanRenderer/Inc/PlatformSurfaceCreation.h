#ifndef SINGULARITY_PLATFORM_SURFACE_CREATION
#define SINGULARITY_PLATFORM_SURFACE_CREATION

#include "Common.h"
#include "VulkanParameters.h"
namespace SingularityEngine::Graphics::Platform
{
	VkResult CreateSurface(StartupParameters startupInfo, VkSurfaceKHR* surface);
}

#endif