#pragma once
#include "Common.h"
#include "Vulkan/Memory/VulkanMemoryObject.h"
namespace SingularityEngine::SERenderer
{
	class VulkanUniformBuffer
	{
	public:
		VulkanUniformBuffer() = default;
		~VulkanUniformBuffer();

		void allocateBuffer();
		void releaseBuffer();
		void* map();
		void unmap();

		const VkDescriptorBufferInfo& getDescriptor() const { return mDescriptor; }
		VkDescriptorBufferInfo& getDescriptor() { return mDescriptor; }
		VkBuffer getBuffer() const { return mBuffer; }
		uint32_t getBindingPoint() const { return mBindingPoint; }
		VkDeviceSize getSize() const { return mSize; }
		VkDeviceMemory getMemory() const { return mMemObject->getDeviceMemory(); }
		const std::string& getName() const { return mName; }
		VkShaderStageFlagBits getStage() const { return mShaderStage; }


		void setStage(VkShaderStageFlagBits stage) { mShaderStage = stage; }
		void setBindingPoint(uint32_t bindingPoint) { mBindingPoint = bindingPoint; }
		void setName(std::string name) { mName = name; }
		void setSize(size_t size) { mSize = size; }

	private:
		std::unique_ptr<VulkanMemoryObject> mMemObject;
		VkBuffer mBuffer = VK_NULL_HANDLE;
		uint32_t mBindingPoint = 0;
		VkDeviceSize mSize = 0;
		VkDescriptorBufferInfo mDescriptor;
		VkShaderStageFlagBits mShaderStage = VK_SHADER_STAGE_FLAG_BITS_MAX_ENUM;
		std::string mName;
	};

}
