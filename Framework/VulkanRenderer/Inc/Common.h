#ifndef SINGULARITY_ENGINE_VULKAN_RENDERER
#define SINGULARITY_ENGINE_VULKAN_RENDERER

#include "External/vulkan/vulkan.h"
#include "External/vulkan/vk_platform.h"
#if defined VK_USE_PLATFORM_WIN32_KHR
#include "External/vulkan/vulkan_win32.h"
#endif

#include "framework/Core/Inc/core.h"
#include "framework/Math/Inc/EngineMath.h"
#include "framework/PhysicsMath/Inc/PhysicsMath.h"
#include <iostream>
#include <optional>
#endif //SINGULARITY_ENGINE_VULKAN_RENDERER