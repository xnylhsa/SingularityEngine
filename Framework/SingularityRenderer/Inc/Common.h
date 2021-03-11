#ifndef SINGULARITY_ENGINE_SINGULARITY_RENDERER
#define SINGULARITY_ENGINE_SINGULARITY_RENDERER
#include "framework/Core/Inc/core.h"
#include "framework/Math/Inc/EngineMath.h"
#include "framework/PhysicsMath/Inc/PhysicsMath.h"
#include <iostream>
#include <optional>




#if defined(SINGULARITY_PLATFORM_ANDROID) || (defined SINGULARITY_PLATFORM_WINDOWS) || defined (SINGULARITY_PLATFORM_MACOS) || defined (SINGULARITY_PLATFORM_IOS)
#define SINGULARITY_USE_VULKAN
#include "External/vulkan/vulkan.h"
#include "External/vulkan/vk_platform.h"
#if defined SINGULARITY_PLATFORM_WINDOWS
#include "External/vulkan/vulkan_win32.h"
#endif


#endif



#endif //SINGULARITY_ENGINE_VULKAN_RENDERER