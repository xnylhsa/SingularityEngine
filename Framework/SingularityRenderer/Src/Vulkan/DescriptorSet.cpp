#include "Precompiled.h"
#include "Vulkan/DescriptorSet.h"
#include "Vulkan/Util/VulkanFunctions.h"
using namespace SingularityEngine::SERenderer;

DescriptorSet::DescriptorSet(VkDevice device, DescriptorPool* descriptorPool, std::vector<VkDescriptorSetLayout> descriptorSetLayouts)
{
	VkDescriptorSetAllocateInfo descriptorSetAllocateInfo =
	{
	VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
	nullptr,
	descriptorPool->get(),
	static_cast<uint32_t>(descriptorSetLayouts.size()),
	descriptorSetLayouts.data()

	};
	mDescriptorSets.resize(descriptorSetLayouts.size());
	ASSERT(vkAllocateDescriptorSets(device, &descriptorSetAllocateInfo, mDescriptorSets.data()) == VK_SUCCESS, "[Graphics::DescriptorSet] Failed to allocate Descriptor sets!");

}

bool DescriptorSet::destroy(VkDevice device, DescriptorPool* descriptorPool)
{
	VkResult result = vkFreeDescriptorSets(device, descriptorPool->get(), static_cast<uint32_t>(mDescriptorSets.size()), mDescriptorSets.data());
	if (VK_SUCCESS != result) {
		LOG("[Graphics::Descriptor Sets]Error occurred during freeing descriptor sets.");
		return false;
	}
	mDescriptorSets.clear();
	return true;
}
