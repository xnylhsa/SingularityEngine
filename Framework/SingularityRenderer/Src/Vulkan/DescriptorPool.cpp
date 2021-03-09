#include "Precompiled.h"
#include "Vulkan/DescriptorPool.h"
#include "Vulkan/Util/VulkanFunctions.h"

using namespace SingularityEngine::SERenderer;

DescriptorPool::DescriptorPool(VkDevice device, std::vector<VkDescriptorPoolSize> descriptorTypes, uint32_t maxSetsCount,  bool freeIndividualSets)
{
	VkDescriptorPoolCreateInfo descriptor_pool_create_info = {
  VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
  nullptr,
  freeIndividualSets ?
	VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT : static_cast<VkDescriptorPoolCreateFlags>(0),
  maxSetsCount,
  static_cast<uint32_t>(descriptorTypes.size()),
  descriptorTypes.data()
	};

	ASSERT(vkCreateDescriptorPool(device, &descriptor_pool_create_info, nullptr, &mDescriptorPool) == VK_SUCCESS, "[Graphics::DescriptorPool] Failed to create Descriptor Pool!");
}


bool DescriptorPool::reset(VkDevice device)
{
	VkResult result = vkResetDescriptorPool(device, mDescriptorPool, 0);
	if (VK_SUCCESS != result) {
		LOG("[Graphics::DescriptorPool] Error occurred during descriptor pool reset.");
		return false;
	}
	return true;
}

bool DescriptorPool::destroy(VkDevice device)
{
	if (VK_NULL_HANDLE != mDescriptorPool) {
		vkDestroyDescriptorPool(device, mDescriptorPool, nullptr);
		mDescriptorPool = VK_NULL_HANDLE;
	}
	return true;
}
