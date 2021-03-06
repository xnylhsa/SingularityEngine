#pragma once
#include "Common.h"
#include "DescriptorPool.h"
#include "DescriptorSetLayout.h"

namespace SingularityEngine::Graphics
{
	class DescriptorSet
	{
	public:
		DescriptorSet(VkDevice device, DescriptorPool* descriptorPool, std::vector<VkDescriptorSetLayout> descriptorSetLayouts);
		VkDescriptorSet get(size_t index) { return mDescriptorSets[index]; }
		bool destroy(VkDevice device, DescriptorPool* descriptorPool);
	private:
		std::vector<VkDescriptorSet> mDescriptorSets;
	};
}
