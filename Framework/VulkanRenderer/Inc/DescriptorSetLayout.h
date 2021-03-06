#pragma once
#include "Common.h"

namespace SingularityEngine::Graphics
{
	class DescriptorSetLayout
	{
	public:
		DescriptorSetLayout(VkDevice device, std::vector<VkDescriptorSetLayoutBinding> bindings);
		bool destroy(VkDevice device);
		~DescriptorSetLayout();
		VkDescriptorSetLayout get() { return mDescriptorSetLayout; }
	private:
		VkDescriptorSetLayout mDescriptorSetLayout = VK_NULL_HANDLE;
	};
}