#pragma once
#include "Common.h"
#include "DescriptorPool.h"
#include "DescriptorSetLayout.h"
#include "UniformBuffer.h"
#include "DescriptorSet.h"
#include "CombinedImageSampler.h"

namespace SingularityEngine::Vulkan {
	class TextureUniformBufferDescriptor
	{
	public:
		bool create(VkPhysicalDevice physicalDevice, VkDevice device, VkExtent3D size, VkDeviceSize uniformBufferSize);
		bool destroy(VkDevice device);
	private:
		UniformBuffer* mUniformBuffer;
		DescriptorPool* mDescriptorPool;
		DescriptorSetLayout* mDescriptorSetLayouts;
		CombinedImageSampler* mCombinedImageSampler;
		DescriptorSet* mDescriptorSets;
	};
}
