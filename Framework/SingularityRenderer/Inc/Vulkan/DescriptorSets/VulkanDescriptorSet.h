#ifndef SINGULARITY_ENGINE_VULKAN_DESCRIPTOR_SET
#define SINGULARITY_ENGINE_VULKAN_DESCRIPTOR_SET
#include "common.h"
namespace SingularityEngine::SERenderer
{
	class VulkanDevice;
	class VulkanDescriptorSetAllocator
	{
	public:
		struct PoolSizes
		{
			std::vector<std::pair<VkDescriptorType, float>> sizes = 
			{
				{VK_DESCRIPTOR_TYPE_SAMPLER, 0.5f},
				{VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 4.0f},
				{VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 4.f},
				{VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1.f},
				{VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1.f},
				{VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1.f},
				{VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 2.f},
				{VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 2.f},
				{VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1.f},
				{VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1.f},
				{VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 0.5f}
			};
		};

		void resetPools();
		bool allocate(VkDescriptorSet* set, VkDescriptorSetLayout layout);
		void init();
		void cleanup();
	private:
		VkDescriptorPool grabPool();
		VkDescriptorPool createPool(int count, VkDescriptorPoolCreateFlags flags);
		std::shared_ptr<VulkanDevice> mDevice = nullptr;
		VkDescriptorPool mCurrentPool = VK_NULL_HANDLE;
		PoolSizes mDescriptorSizes;
		std::vector<VkDescriptorPool> mUsedPools;
		std::vector<VkDescriptorPool> mFreePools;
	};


	class VulkanDescriptorSetLayoutCache 
	{
	public:
		void init();
		void cleanup();

		VkDescriptorSetLayout createDescriptorLayout(VkDescriptorSetLayoutCreateInfo* info);

		struct VulkanDescripterLayoutInfo
		{
			std::vector<VkDescriptorSetLayoutBinding> bindings;
			bool operator==(const VulkanDescripterLayoutInfo& other) const;
			size_t hash() const;
		}; 
	private:
		struct VulkanDescriptorLayoutHash
		{
			std::size_t operator()(const VulkanDescripterLayoutInfo& k) const {
				return k.hash();
			}
		};

		std::unordered_map<VulkanDescripterLayoutInfo, VkDescriptorSetLayout, VulkanDescriptorLayoutHash> mLayoutCache;
		std::shared_ptr<VulkanDevice> mDevice = nullptr;
	};

	class VulkanDescriptorBuilder
	{
	public:
		static VulkanDescriptorBuilder begin(VulkanDescriptorSetLayoutCache* layoutCache, VulkanDescriptorSetAllocator* allocator);
		VulkanDescriptorBuilder& bindBuffer(uint32_t binding, VkDescriptorBufferInfo* bufferInfo, VkDescriptorType type, VkShaderStageFlags stageFlags);
		VulkanDescriptorBuilder& bindImage(uint32_t binding, VkDescriptorImageInfo* bufferInfo, VkDescriptorType type, VkShaderStageFlags stageFlags);
		bool build(VkDescriptorSet& set, VkDescriptorSetLayout& layout);
		bool build(VkDescriptorSet& set);
	private:
		std::vector<VkWriteDescriptorSet> mWrites;
		std::vector<VkDescriptorSetLayoutBinding> mbindings;
		VulkanDescriptorSetLayoutCache* mCache;
		VulkanDescriptorSetAllocator* mAllocator;
	};

}

#endif