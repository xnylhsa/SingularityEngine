#ifndef SINGULARITY_ENGINE_VULKAN_DESCRIPTOR_SET
#define SINGULARITY_ENGINE_VULKAN_DESCRIPTOR_SET
#include "common.h"
#include "Vulkan/Texture/VulkanSampler.h"
namespace SingularityEngine::SERenderer
{
	enum class BindingType
	{
		None,
		SampledImage,
		StorageImage,
		UniformBuffer,
		StorageBuffer,
		SampledBuffer,
		SeperateImage,
		Sampler,
		InputAttachment,
		Count
	};

	struct BindingInfo
	{
		BindingType type = BindingType::None;
		uint8_t size = 0;
		bool isFloatingPoint = false;
		SamplerTypes sampler = SamplerTypes::Count;
		bool isImutableSampler = false;
	};

	struct DescriptorSetLayout
	{
		BindingInfo bindingInfos[VULKAN_NUM_BINDINGS] = {};
	};

	class VulkanDescriptorSetAllocator
	{
	public:
		VulkanDescriptorSetAllocator(const DescriptorSetLayout& layout, const uint32_t* stagesForBinding);
		~VulkanDescriptorSetAllocator();
		void operator=(const VulkanDescriptorSetAllocator&) = delete;
		VulkanDescriptorSetAllocator(const VulkanDescriptorSetAllocator&) = delete;
		VkDescriptorSetLayout getLayout() { return mSetLayout; }

		void clear();
		VkDescriptorSet allocateBindlessSet(VkDescriptorPool pool, size_t numDescriptors);
		VkDescriptorPool allocateBindlessPool(size_t numSets, size_t numDescriptors);

	private:
		VkDescriptorSetLayout mSetLayout = VK_NULL_HANDLE;
		std::unordered_map<uint64_t, VkDescriptorSet> mSets;
		std::vector<VkDescriptorPoolSize> mPoolSizes;

		std::vector<VkDescriptorPool> mPools;
		bool mBindless = false;
	};


	class VulkanBindlessDescriptorPool
	{
	public:
		VulkanBindlessDescriptorPool(VulkanDescriptorSetAllocator* allocator, VkDescriptorPool pool);
		~VulkanBindlessDescriptorPool();

		void operator=(const VulkanBindlessDescriptorPool & other) = delete;
		VulkanBindlessDescriptorPool(const VulkanBindlessDescriptorPool& other) = delete;

		bool allocateDescriptors(size_t count);
		VkDescriptorSet getDescriptorSet() const;

		//void setTexture(size_t binding, const ImageView& view);
		//void setTexture_unorm(size_t binding, const ImageView& view);
		//void setTexture_srgb(size_t binding, const ImageView& view);
	private:
		VulkanDescriptorSetAllocator* mAllocator;
		VkDescriptorPool mDescriptorPool;
		VkDescriptorSet mDescriptorSet;

		void setTexture(size_t binding, VkImageView view, VkImageLayout layout);
	};



}

#endif