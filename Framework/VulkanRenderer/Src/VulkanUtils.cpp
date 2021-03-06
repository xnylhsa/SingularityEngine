#include "Precompiled.h"
#include "VulkanUtils.h"

using namespace SingularityEngine::Graphics;

VkClearColorValue VulkenUtil::Convert(Math::Color color)
{
	return VkClearColorValue({ color.r, color.g, color.b, color.a });
}
