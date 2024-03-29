#include "Precompiled.h"
#include "Vulkan/Shader/VulkanShaderModule.h"
#include "Vulkan/Util/VulkanFunctions.h"

using namespace SingularityEngine::SERenderer;

ShaderModule::ShaderModule(VkDevice device, std::vector<unsigned char> sourceCode)
{
	VkShaderModuleCreateInfo createInfo = {
		VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
		nullptr,
		0,
		static_cast<uint32_t>(sourceCode.size()),
		reinterpret_cast<uint32_t const*>(sourceCode.data())
	};

	ASSERT(vkCreateShaderModule(device, &createInfo, nullptr, &mShaderModule) == VK_SUCCESS, "[Graphics::Graphics::Shader] Failed to create Shader!");
	return;
}

