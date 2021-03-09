#ifndef SINGULARITY_PLATFORM_SURFACE_CREATION
#define SINGULARITY_PLATFORM_SURFACE_CREATION

#include "Common.h"
#include "Vulkan/Util/VulkanParameters.h"
namespace SingularityEngine::SERenderer::Platform
{
	VkResult CreateSurface(VkSurfaceKHR* surface);
}

#endif