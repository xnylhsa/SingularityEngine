#pragma once
#include "Common.h"

namespace SingularityEngine::Graphics
{
	class DescriptorPool
	{
	public:
		DescriptorPool(VkDevice device, std::vector<VkDescriptorPoolSize> descriptorTypes, uint32_t maxSetsCount, bool freeIndividualSets);
		bool destroy(VkDevice device);
		bool reset(VkDevice device);
		VkDescriptorPool get() { return mDescriptorPool; }
	private:
		VkDescriptorPool mDescriptorPool;
	};
}

