#include "Precompiled.h"
#include "Vulkan/Command/VulkanCommandPool.h"
#include "Vulkan/Util/VulkanFunctions.h"
#include "Vulkan/Device/VulkanDevice.h"
namespace SingularityEngine::SERenderer
{


	VulkanCommandPool::VulkanCommandPool()
	{

	}

	VulkanCommandPool::VulkanCommandPool(std::weak_ptr<VulkanDevice> device, uint32_t queueFamilyIndex) : mDevice(device), mQueueFamilyIndex(queueFamilyIndex)
	{

	}

	VulkanCommandPool::VulkanCommandPool(VulkanCommandPool&& other) noexcept
	{
		*this = std::move(other);
	}

	bool VulkanCommandPool::create(std::weak_ptr<VulkanDevice> device, uint32_t queueFamilyIndex)
	{
		mDevice = device;
		auto vulkanDevice = mDevice.lock();
		ASSERT(vulkanDevice, "[SERenderer::VulkanImage] device lost.");
		mQueueFamilyIndex = queueFamilyIndex;


		VkCommandPoolCreateInfo cmdPoolCreateInfo = {
		VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
		nullptr,
		VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT | VK_COMMAND_POOL_CREATE_TRANSIENT_BIT,
		*mQueueFamilyIndex
		};

		if (vkCreateCommandPool(*vulkanDevice, &cmdPoolCreateInfo, nullptr, &mCommandPool) != VK_SUCCESS)
		{
			LOG("[Graphics::CommandPool] Failed creation.");
			return false;
		}

		return true;
	}

	bool VulkanCommandPool::destroy()
	{
		auto vulkanDevice = mDevice.lock();
		ASSERT(vulkanDevice, "[SERenderer::VulkanImage] device lost.");
		if (vulkanDevice) {
			if (!mBuffers.empty())
			{
				vkFreeCommandBuffers(vulkanDevice->getLogicalDevice(), mCommandPool, (uint32_t)mBuffers.size(), mBuffers.data());
			}
			if (!mSecondaryBuffers.empty())
			{
				vkFreeCommandBuffers(vulkanDevice->getLogicalDevice(), mCommandPool, (uint32_t)mSecondaryBuffers.size(), mSecondaryBuffers.data());
			}
			if (mCommandPool != VK_NULL_HANDLE)
			{
				vkDestroyCommandPool(vulkanDevice->getLogicalDevice(), mCommandPool, nullptr);
				mCommandPool = VK_NULL_HANDLE;
			}
			mDevice.reset();
		}
		return true;
	}

	void VulkanCommandPool::begin()
	{
		auto vulkanDevice = mDevice.lock();
		ASSERT(vulkanDevice, "[SERenderer::VulkanImage] device lost.");
		if (vkResetCommandPool(*vulkanDevice, mCommandPool, 0) != VK_SUCCESS)
		{
			LOG("[Graphics::Comannd Pool] Error occured during reset!");
		}
		mIndex = 0;
		mSecondaryIndex = 0;
	}

	VulkanCommandPool& VulkanCommandPool::operator=(VulkanCommandPool&& other) noexcept
	{
		if (this != &other)
		{
			auto vulkanDevice = mDevice.lock();
			ASSERT(vulkanDevice, "[SERenderer::VulkanImage] device lost.");
			mDevice = other.mDevice;
			if (!mBuffers.empty())
				vkFreeCommandBuffers(vulkanDevice->getLogicalDevice(), mCommandPool, (uint32_t)mBuffers.size(), mBuffers.data());
			if (mCommandPool != VK_NULL_HANDLE)
				vkDestroyCommandPool(vulkanDevice->getLogicalDevice(), mCommandPool, nullptr);

			mCommandPool = VK_NULL_HANDLE;
			mBuffers.clear();
			std::swap(mCommandPool, other.mCommandPool);
			std::swap(mBuffers, other.mBuffers);
			mIndex = other.mIndex;
			other.mIndex = 0;
		}
		return *this;
	}

	VkCommandBuffer VulkanCommandPool::requestCommandBuffer()
	{
		if (mIndex < mBuffers.size())
		{
			auto ret = mBuffers[mIndex++];
			return ret;
		}
		auto ret = allocateCommandBuffer();
		mBuffers.push_back(ret);
		mIndex++;
		return ret;
	}

	VkCommandBuffer VulkanCommandPool::requestSecondaryCommandBuffer()
	{
		if (mIndex < mBuffers.size())
		{
			auto ret = mSecondaryBuffers[mSecondaryIndex++];
			return ret;
		}
		auto ret = allocateCommandBuffer(true);
		mSecondaryBuffers.push_back(ret);
		mSecondaryIndex++;
		return ret;
	}

	VkCommandBuffer VulkanCommandPool::allocateCommandBuffer(bool isSecondary /*= false*/)
	{
		auto vulkanDevice = mDevice.lock();
		ASSERT(vulkanDevice, "[SERenderer::VulkanImage] device lost.");
		VkCommandBuffer cmdBuffer;
		VkCommandBufferAllocateInfo info = {};
		info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		info.commandPool = mCommandPool;
		info.level = isSecondary ? VK_COMMAND_BUFFER_LEVEL_SECONDARY : VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		info.commandBufferCount = 1;
		vkAllocateCommandBuffers(vulkanDevice->getLogicalDevice(), &info, &cmdBuffer);
		return cmdBuffer;
	}

	void VulkanCommandPool::trim()
	{
		auto vulkanDevice = mDevice.lock();
		ASSERT(vulkanDevice, "[SERenderer::VulkanImage] device lost.");
		vkResetCommandPool(vulkanDevice->getLogicalDevice(), mCommandPool, VK_COMMAND_POOL_RESET_RELEASE_RESOURCES_BIT);
		if (!mBuffers.empty())
			vkFreeCommandBuffers(vulkanDevice->getLogicalDevice(), mCommandPool, (uint32_t)mBuffers.size(), mBuffers.data());
		if (!mSecondaryBuffers.empty())
			vkFreeCommandBuffers(vulkanDevice->getLogicalDevice(), mCommandPool, (uint32_t)mSecondaryBuffers.size(), mSecondaryBuffers.data());
		mBuffers.clear();
		mSecondaryBuffers.clear();
		vkTrimCommandPool(vulkanDevice->getLogicalDevice(), mCommandPool, 0);
	}

}