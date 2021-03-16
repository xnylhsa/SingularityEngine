#include "Precompiled.h"
#include "vulkan/DescriptorSets/VulkanDescriptorSet.h"
#include "vulkan/Device/VulkanDevice.h"
#include "RendererAPI/Renderer.h"
#include "vulkan/Util/VulkanFunctions.h"
#include "vulkan/Texture/VulkanSampler.h"
namespace SingularityEngine::SERenderer
{

	VulkanDescriptorSetAllocator::VulkanDescriptorSetAllocator(const DescriptorSetLayout& layout, const uint32_t* stagesForBinding)
	{
		std::shared_ptr<VulkanDevice> device = std::dynamic_pointer_cast<VulkanDevice>(Renderer::Get()->getGraphicsDevice());

		mBindless = layout.bindingInfos[0].size == VULKAN_UNBOUND_ARRAY;

		VkDescriptorSetLayoutCreateInfo info = { VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO };
		VkDescriptorSetLayoutBindingFlagsCreateInfoEXT flags = { VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_BINDING_FLAGS_CREATE_INFO_EXT };
		std::vector<VkDescriptorSetLayoutBinding> bindings;
		VkDescriptorBindingFlagsEXT binding_flags = 0;

		if (mBindless)
		{
			info.flags |= VK_DESCRIPTOR_SET_LAYOUT_CREATE_UPDATE_AFTER_BIND_POOL_BIT_EXT;
			info.pNext = &flags;

			flags.bindingCount = 1;
			flags.pBindingFlags = &binding_flags;
			binding_flags = VK_DESCRIPTOR_BINDING_PARTIALLY_BOUND_BIT_EXT |
				VK_DESCRIPTOR_BINDING_UPDATE_AFTER_BIND_BIT_EXT;
			binding_flags |= VK_DESCRIPTOR_BINDING_VARIABLE_DESCRIPTOR_COUNT_BIT_EXT;
		}

		for (unsigned i = 0; i < VULKAN_NUM_BINDINGS; i++)
		{
			auto stages = stagesForBinding[i];
			if (stages == 0)
				continue;

			unsigned array_size = layout.bindingInfos[i].size;
			unsigned pool_array_size;
			if (array_size == VULKAN_UNBOUND_ARRAY)
			{
				array_size = VULKAN_NUM_BINDINGS_BINDLESS_VARYING;
				pool_array_size = array_size;
			}
			else
				pool_array_size = array_size * VULKAN_NUM_SETS_PER_POOL;

			switch (layout.bindingInfos[i].type)
			{
			case BindingType::SampledImage:
			{
				VkSampler sampler = VK_NULL_HANDLE;
				if (layout.bindingInfos[i].isImutableSampler)
					sampler = device->getStockSampler(layout.bindingInfos[i].sampler)->getSampler();

				bindings.push_back({ i, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, array_size, stages, sampler != VK_NULL_HANDLE ? &sampler : nullptr });
				mPoolSizes.push_back({ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, pool_array_size });
				break;
			}
			case BindingType::StorageImage:
				bindings.push_back({ i, VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, array_size, stages, nullptr });
				mPoolSizes.push_back({ VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, pool_array_size });
				break;
			case BindingType::UniformBuffer:
				bindings.push_back({ i, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, array_size, stages, nullptr });
				mPoolSizes.push_back({ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, pool_array_size });
				break;
			case BindingType::StorageBuffer:
				bindings.push_back({ i, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, array_size, stages, nullptr });
				mPoolSizes.push_back({ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, pool_array_size });
				break;
			case BindingType::SampledBuffer:
				bindings.push_back({ i, VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, array_size, stages, nullptr });
				mPoolSizes.push_back({ VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, pool_array_size });
				break;
			case BindingType::SeperateImage:
				bindings.push_back({ i, VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, array_size, stages, nullptr });
				mPoolSizes.push_back({ VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, pool_array_size });
				break;
			case BindingType::Sampler:
			{

				VkSampler sampler = VK_NULL_HANDLE;
				if (layout.bindingInfos[i].isImutableSampler)
					sampler = device->getStockSampler(layout.bindingInfos[i].sampler)->getSampler(); // have device create a set of stock samplers on creation, then reach in to unorderedmap grab sampler wanted from enum

				bindings.push_back({ i, VK_DESCRIPTOR_TYPE_SAMPLER, array_size, stages, sampler != VK_NULL_HANDLE ? &sampler : nullptr });
				mPoolSizes.push_back({ VK_DESCRIPTOR_TYPE_SAMPLER, pool_array_size });
				break;
			}
			case BindingType::InputAttachment:
				bindings.push_back({ i, VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, array_size, stages, nullptr });
				mPoolSizes.push_back({ VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, pool_array_size });
				break;
			default:
				break;
			}
		}

		if (!bindings.empty())
		{
			info.bindingCount = (uint32_t)bindings.size();
			info.pBindings = bindings.data();

			if (mBindless && bindings.size() != 1)
			{
				LOG("Using bindless but have bindingCount != 1.\n");
				return;
			}
		}
		if (vkCreateDescriptorSetLayout(device->getLogicalDevice(), &info, nullptr, &mSetLayout) != VK_SUCCESS)
			LOG("Failed to create descriptor set layout.");
	}

	VkDescriptorSet VulkanDescriptorSetAllocator::allocateBindlessSet(VkDescriptorPool pool, size_t numDescriptors)
	{
		if (!pool || !mBindless)
			return VK_NULL_HANDLE;
		std::shared_ptr<VulkanDevice> device = std::dynamic_pointer_cast<VulkanDevice>(Renderer::Get()->getGraphicsDevice());

		VkDescriptorSetAllocateInfo info = { VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO };
		info.descriptorPool = pool;
		info.descriptorSetCount = 1;
		info.pSetLayouts = &mSetLayout;

		VkDescriptorSetVariableDescriptorCountAllocateInfoEXT count_info =
		{ VK_STRUCTURE_TYPE_DESCRIPTOR_SET_VARIABLE_DESCRIPTOR_COUNT_ALLOCATE_INFO_EXT };

		auto num_desc = (uint32_t)numDescriptors;
		count_info.descriptorSetCount = 1;
		count_info.pDescriptorCounts = &num_desc;
		info.pNext = &count_info;

		VkDescriptorSet desc_set = VK_NULL_HANDLE;
		if (vkAllocateDescriptorSets(device->getLogicalDevice(), &info, &desc_set) != VK_SUCCESS)
			return VK_NULL_HANDLE;

		return desc_set;
	}

	VkDescriptorPool VulkanDescriptorSetAllocator::allocateBindlessPool(size_t numSets, size_t numDescriptors)
	{
		if (!mBindless)
			return VK_NULL_HANDLE;
		std::shared_ptr<VulkanDevice> device = std::dynamic_pointer_cast<VulkanDevice>(Renderer::Get()->getGraphicsDevice());

		VkDescriptorPool pool = VK_NULL_HANDLE;
		VkDescriptorPoolCreateInfo info = { VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO };
		info.flags = VK_DESCRIPTOR_POOL_CREATE_UPDATE_AFTER_BIND_BIT_EXT;
		info.maxSets = (uint32_t)numSets;
		info.poolSizeCount = 1;

		VkDescriptorPoolSize size = mPoolSizes[0];
		if (numDescriptors > size.descriptorCount)
		{
			LOG("Trying to allocate more than max bindless descriptors for descriptor layout.\n");
			return VK_NULL_HANDLE;
		}

		size.descriptorCount = (uint32_t)numDescriptors;

		info.pPoolSizes = &size;

		if (vkCreateDescriptorPool(device->getLogicalDevice(), &info, nullptr, &pool) != VK_SUCCESS)
		{
			LOG("Failed to create descriptor pool.\n");
			return VK_NULL_HANDLE;
		}

		return pool;
	}

	void VulkanDescriptorSetAllocator::clear()
	{
		std::shared_ptr<VulkanDevice> device = std::dynamic_pointer_cast<VulkanDevice>(Renderer::Get()->getGraphicsDevice());
		mSets.clear();
		for (auto& pool : mPools)
		{
			vkResetDescriptorPool(device->getLogicalDevice(), pool, 0);
			vkDestroyDescriptorPool(device->getLogicalDevice(), pool, nullptr);
		}
		mPools.clear();
	}

	VulkanDescriptorSetAllocator::~VulkanDescriptorSetAllocator()
	{
		std::shared_ptr<VulkanDevice> device = std::dynamic_pointer_cast<VulkanDevice>(Renderer::Get()->getGraphicsDevice());

		if (mSetLayout != VK_NULL_HANDLE)
			vkDestroyDescriptorSetLayout(device->getLogicalDevice(), mSetLayout, nullptr);
		clear();
	}

	VulkanBindlessDescriptorPool::VulkanBindlessDescriptorPool(VulkanDescriptorSetAllocator* allocator, VkDescriptorPool pool) : mAllocator(allocator), mDescriptorPool(pool)
	{

	}

	VulkanBindlessDescriptorPool::~VulkanBindlessDescriptorPool()
	{
		if (mDescriptorPool)
		{
			std::shared_ptr<VulkanDevice> device = std::dynamic_pointer_cast<VulkanDevice>(Renderer::Get()->getGraphicsDevice());
			vkDestroyDescriptorPool(device->getLogicalDevice(), mDescriptorPool, nullptr);
		}
	}

	bool VulkanBindlessDescriptorPool::allocateDescriptors(size_t count)
	{
		mDescriptorSet = mAllocator->allocateBindlessSet(mDescriptorPool, count);
		return mDescriptorSet != VK_NULL_HANDLE;
	}

	VkDescriptorSet VulkanBindlessDescriptorPool::getDescriptorSet() const
	{
		return mDescriptorSet;
	}



	void VulkanBindlessDescriptorPool::setTexture(size_t binding, VkImageView view, VkImageLayout layout)
	{
		VkWriteDescriptorSet write = { VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET };
		write.descriptorCount = 1;
		write.dstArrayElement = (uint32_t)binding;
		write.descriptorType = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
		write.dstSet = mDescriptorSet;

		const VkDescriptorImageInfo info = {
			VK_NULL_HANDLE,
			view,
			layout,
		};
		write.pImageInfo = &info;
		std::shared_ptr<VulkanDevice> device = std::dynamic_pointer_cast<VulkanDevice>(Renderer::Get()->getGraphicsDevice());

		vkUpdateDescriptorSets(device->getLogicalDevice(), 1, &write, 0, nullptr);

	}

}