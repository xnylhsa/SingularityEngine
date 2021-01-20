#include "Precompiled.h"
#include "DescriptorSetLayout.h"
#include "VulkanFunctions.h"

using namespace SingularityEngine::Vulkan;

DescriptorSetLayout::DescriptorSetLayout(VkDevice device, std::vector<VkDescriptorSetLayoutBinding> bindings)
{
	VkDescriptorSetLayoutCreateInfo descriptorSetLayoutCreateInfo =
	{
	 VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
	 nullptr,
	 0,
	 static_cast<uint32_t>(bindings.size()),
	 bindings.data()
	};
	VkResult result = vkCreateDescriptorSetLayout(device, &descriptorSetLayoutCreateInfo, nullptr, &mDescriptorSetLayout);
	ASSERT(result == VK_SUCCESS, "[Graphics::DescriptorSet] Could not create a layout for descriptor sets.");
}

bool DescriptorSetLayout::destroy(VkDevice device)
{
	if (VK_NULL_HANDLE != mDescriptorSetLayout) {
		vkDestroyDescriptorSetLayout(device, mDescriptorSetLayout, nullptr);
		mDescriptorSetLayout = VK_NULL_HANDLE;
	}
	return true;
}

DescriptorSetLayout::~DescriptorSetLayout()
{

}
