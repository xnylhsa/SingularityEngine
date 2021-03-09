#include "Precompiled.h"
#include "Vulkan/Util/VulkanUtils.h"

using namespace SingularityEngine::SERenderer;

VkClearColorValue VulkenUtil::Convert(Math::Color color)
{
	return VkClearColorValue({ color.r, color.g, color.b, color.a });
}
