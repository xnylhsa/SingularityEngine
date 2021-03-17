#include "Precompiled.h"
#include "vulkan/DescriptorSets/VulkanDescriptorSet.h"
#include "vulkan/Device/VulkanDevice.h"
#include "RendererAPI/Renderer.h"
#include "vulkan/Util/VulkanFunctions.h"
#include "vulkan/Texture/VulkanSampler.h"
namespace SingularityEngine::SERenderer
{
	void VulkanDescriptorSetAllocator::resetPools()
	{
		for (auto p : mUsedPools) {
			vkResetDescriptorPool(mDevice->getLogicalDevice(), p, 0);
		}
		//move all pools to the reusable vector
		mFreePools = mUsedPools;
		mUsedPools.clear();
		//reset the current pool handle back to null
		mCurrentPool = VK_NULL_HANDLE;
	}

	bool VulkanDescriptorSetAllocator::allocate(VkDescriptorSet* set, VkDescriptorSetLayout layout)
	{
		//initialize the currentPool handle if it's null
		if (mCurrentPool == VK_NULL_HANDLE) {

			mCurrentPool = grabPool();
			mUsedPools.push_back(mCurrentPool);
		}

		VkDescriptorSetAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocInfo.pNext = nullptr;

		allocInfo.pSetLayouts = &layout;
		allocInfo.descriptorPool = mCurrentPool;
		allocInfo.descriptorSetCount = 1;

		//try to allocate the descriptor set
		VkResult allocResult = vkAllocateDescriptorSets(mDevice->getLogicalDevice(), &allocInfo, set);
		bool needReallocate = false;

		switch (allocResult) {
		case VK_SUCCESS:
			//all good, return
			return true;
		case VK_ERROR_FRAGMENTED_POOL:
		case VK_ERROR_OUT_OF_POOL_MEMORY:
			//reallocate pool
			needReallocate = true;
			break;
		default:
			//unrecoverable error
			return false;
		}

		if (needReallocate) {
			//allocate a new pool and retry
			mCurrentPool = grabPool();
			mUsedPools.push_back(mCurrentPool);

			allocResult = vkAllocateDescriptorSets(mDevice->getLogicalDevice(), &allocInfo, set);

			//if it still fails then we have big issues
			if (allocResult == VK_SUCCESS) {
				return true;
			}
		}

		return false;
	}

	void VulkanDescriptorSetAllocator::init()
	{
		mDevice = std::dynamic_pointer_cast<VulkanDevice>(Renderer::Get()->getGraphicsDevice());
		ASSERT(mDevice, "[SERenderer::VulkanDescriptorSetAllocator] Device was lost.");
	}

	void VulkanDescriptorSetAllocator::cleanup()
	{
		for (auto p : mFreePools)
		{
			vkDestroyDescriptorPool(mDevice->getLogicalDevice(), p, nullptr);
		}
		for (auto p : mUsedPools)
		{
			vkDestroyDescriptorPool(mDevice->getLogicalDevice(), p, nullptr);
		}
		mDevice.reset();
	}

	VkDescriptorPool VulkanDescriptorSetAllocator::grabPool()
	{
		if (!mFreePools.empty())
		{
			VkDescriptorPool pool = mFreePools.back();
			mFreePools.pop_back();
			return pool;
		}
		return createPool(1000, 0);
	}

	VkDescriptorPool VulkanDescriptorSetAllocator::createPool(int count, VkDescriptorPoolCreateFlags flags)
	{
		std::vector<VkDescriptorPoolSize> sizes;
		sizes.reserve(mDescriptorSizes.sizes.size());
		for (auto sz : mDescriptorSizes.sizes) {
			sizes.push_back({ sz.first, uint32_t(sz.second * count) });
		}
		VkDescriptorPoolCreateInfo pool_info = {};
		pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		pool_info.flags = flags;
		pool_info.maxSets = count;
		pool_info.poolSizeCount = (uint32_t)sizes.size();
		pool_info.pPoolSizes = sizes.data();

		VkDescriptorPool descriptorPool;
		vkCreateDescriptorPool(mDevice->getLogicalDevice(), &pool_info, nullptr, &descriptorPool);

		return descriptorPool;
	}

	void VulkanDescriptorSetLayoutCache::init()
	{
		mDevice = std::dynamic_pointer_cast<VulkanDevice>(Renderer::Get()->getGraphicsDevice());
		ASSERT(mDevice, "[SERenderer::VulkanDescriptorSetAllocator] Device was lost.");
	}

	void VulkanDescriptorSetLayoutCache::cleanup()
	{
		for (auto pair : mLayoutCache) {
			vkDestroyDescriptorSetLayout(mDevice->getLogicalDevice(), pair.second, nullptr);
		}
		mDevice.reset();
	}

	VkDescriptorSetLayout VulkanDescriptorSetLayoutCache::createDescriptorLayout(VkDescriptorSetLayoutCreateInfo* info)
	{
		VulkanDescripterLayoutInfo layoutinfo;
		layoutinfo.bindings.reserve(info->bindingCount);
		bool isSorted = true;
		int lastBinding = -1;

		//copy from the direct info struct into our own one
		for (uint32_t i = 0; i < info->bindingCount; i++) {
			layoutinfo.bindings.push_back(info->pBindings[i]);

			//check that the bindings are in strict increasing order
			if ((int)info->pBindings[i].binding > lastBinding) {
				lastBinding = info->pBindings[i].binding;
			}
			else {
				isSorted = false;
			}
		}
		//sort the bindings if they arent in order
		if (!isSorted) {
			std::sort(layoutinfo.bindings.begin(), layoutinfo.bindings.end(), [](VkDescriptorSetLayoutBinding& a, VkDescriptorSetLayoutBinding& b) {
				return a.binding < b.binding;
				});
		}

		//try to grab from cache
		auto it = mLayoutCache.find(layoutinfo);
		if (it != mLayoutCache.end()) {
			return (*it).second;
		}
		//create a new one (not found)
		VkDescriptorSetLayout layout;
		vkCreateDescriptorSetLayout(mDevice->getLogicalDevice(), info, nullptr, &layout);

		//add to cache
		mLayoutCache[layoutinfo] = layout;
		return layout;
	}

	bool VulkanDescriptorSetLayoutCache::VulkanDescripterLayoutInfo::operator==(const VulkanDescripterLayoutInfo& other) const
	{
		if (other.bindings.size() != bindings.size()) {
			return false;
		}
		else {
			//compare each of the bindings is the same. Bindings are sorted so they will match
			for (int i = 0; i < bindings.size(); i++) {
				if (other.bindings[i].binding != bindings[i].binding) {
					return false;
				}
				if (other.bindings[i].descriptorType != bindings[i].descriptorType) {
					return false;
				}
				if (other.bindings[i].descriptorCount != bindings[i].descriptorCount) {
					return false;
				}
				if (other.bindings[i].stageFlags != bindings[i].stageFlags) {
					return false;
				}
			}
			return true;
		}
	}

	size_t VulkanDescriptorSetLayoutCache::VulkanDescripterLayoutInfo::hash() const
	{
		using std::size_t;
		using std::hash;

		size_t result = hash<size_t>()(bindings.size());

		for (const VkDescriptorSetLayoutBinding& b : bindings)
		{
			//pack the binding data into a single int64. Not fully correct but it's ok
			size_t binding_hash = b.binding | b.descriptorType << 8 | b.descriptorCount << 16 | b.stageFlags << 24;

			//shuffle the packed binding data and xor it with the main hash
			result ^= hash<size_t>()(binding_hash);
		}

		return result;
	}

	VulkanDescriptorBuilder VulkanDescriptorBuilder::begin(VulkanDescriptorSetLayoutCache* layoutCache, VulkanDescriptorSetAllocator* allocator)
	{
		VulkanDescriptorBuilder builder;

		builder.mCache = layoutCache;
		builder.mAllocator = allocator;
		return builder;
	}

	VulkanDescriptorBuilder& VulkanDescriptorBuilder::bindBuffer(uint32_t binding, VkDescriptorBufferInfo* bufferInfo, VkDescriptorType type, VkShaderStageFlags stageFlags)
	{
		//create the descriptor binding for the layout
		VkDescriptorSetLayoutBinding newBinding{};

		newBinding.descriptorCount = 1;
		newBinding.descriptorType = type;
		newBinding.pImmutableSamplers = nullptr;
		newBinding.stageFlags = stageFlags;
		newBinding.binding = binding;

		mbindings.push_back(newBinding);

		//create the descriptor write
		VkWriteDescriptorSet newWrite{};
		newWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		newWrite.pNext = nullptr;

		newWrite.descriptorCount = 1;
		newWrite.descriptorType = type;
		newWrite.pBufferInfo = bufferInfo;
		newWrite.dstBinding = binding;

		mWrites.push_back(newWrite);
		return *this;
	}

	VulkanDescriptorBuilder& VulkanDescriptorBuilder::bindImage(uint32_t binding, VkDescriptorImageInfo* bufferInfo, VkDescriptorType type, VkShaderStageFlags stageFlags)
	{
		//create the descriptor binding for the layout
		VkDescriptorSetLayoutBinding newBinding{};

		newBinding.descriptorCount = 1;
		newBinding.descriptorType = type;
		newBinding.pImmutableSamplers = nullptr;
		newBinding.stageFlags = stageFlags;
		newBinding.binding = binding;

		mbindings.push_back(newBinding);

		//create the descriptor write
		VkWriteDescriptorSet newWrite{};
		newWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		newWrite.pNext = nullptr;
		newWrite.descriptorCount = 1;
		newWrite.descriptorType = type;
		newWrite.pImageInfo = bufferInfo;
		newWrite.dstBinding = binding;

		mWrites.push_back(newWrite);
		return *this;
	}

	bool VulkanDescriptorBuilder::build(VkDescriptorSet& set, VkDescriptorSetLayout& layout)
	{
		std::shared_ptr<VulkanDevice> device = std::dynamic_pointer_cast<VulkanDevice>(Renderer::Get()->getGraphicsDevice());

		VkDescriptorSetLayoutCreateInfo layoutInfo{};
		layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		layoutInfo.pNext = nullptr;

		layoutInfo.pBindings = mbindings.data();
		layoutInfo.bindingCount = (uint32_t)mbindings.size();

		layout = mCache->createDescriptorLayout(&layoutInfo);

		//allocate descriptor
		bool success = mAllocator->allocate(&set, layout);
		if (!success) { return false; };

		//write descriptor
		for (VkWriteDescriptorSet& w : mWrites) {
			w.dstSet = set;
		}

		vkUpdateDescriptorSets(device->getLogicalDevice(), (uint32_t)mWrites.size(), mWrites.data(), 0, nullptr);

		return true;
	}

	bool VulkanDescriptorBuilder::build(VkDescriptorSet& set)
	{
		std::shared_ptr<VulkanDevice> device = std::dynamic_pointer_cast<VulkanDevice>(Renderer::Get()->getGraphicsDevice());

		VkDescriptorSetLayoutCreateInfo layoutInfo{};
		layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		layoutInfo.pNext = nullptr;

		layoutInfo.pBindings = mbindings.data();
		layoutInfo.bindingCount = (uint32_t)mbindings.size();

		VkDescriptorSetLayout layout = mCache->createDescriptorLayout(&layoutInfo);

		//allocate descriptor
		bool success = mAllocator->allocate(&set, layout);
		if (!success) { return false; };

		//write descriptor
		for (VkWriteDescriptorSet& w : mWrites) {
			w.dstSet = set;
		}

		vkUpdateDescriptorSets(device->getLogicalDevice(), (uint32_t)mWrites.size(), mWrites.data(), 0, nullptr);

		return true;
	}

}